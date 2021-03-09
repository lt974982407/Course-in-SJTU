#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <map>
#include <vector>
#include "semant.h"
#include "utilities.h"

extern int semant_debug;
extern char *curr_filename;

static ostream& error_stream = cerr;
static int semant_errors = 0;
static Decl curr_decl = 0;

typedef SymbolTable<Symbol, Symbol> ObjectEnvironment; // name, type
ObjectEnvironment objectEnv;

typedef std::map <Symbol, Symbol> ig;

ig calldecl_list;
ig globalvar_list;
typedef std::vector<Symbol> svector;
ig para_list;
ig pt;
std::vector <ig> local;
svector para;
std:: map<Symbol,svector> funpara;
int num = 0;


///////////////////////////////////////////////
// helper func
///////////////////////////////////////////////


static ostream& semant_error() {
    semant_errors++;
    return error_stream;
}

static ostream& semant_error(tree_node *t) {
    error_stream << t->get_line_number() << ": ";
    return semant_error();
}

static ostream& internal_error(int lineno) {
    error_stream << "FATAL:" << lineno << ": ";
    return error_stream;
}

//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////

static Symbol 
    Int,
    Float,
    String,
    Bool,
    Void,
    Main,
    print
    ;

bool isValidCallName(Symbol type) {
    return type != (Symbol)print;
}

bool isValidTypeName(Symbol type) {
    return type != Void;
}

//
// Initializing the predefined symbols.
//

static void initialize_constants(void) {
    // 4 basic types and Void type
    Bool        = idtable.add_string("Bool");
    Int         = idtable.add_string("Int");
    String      = idtable.add_string("String");
    Float       = idtable.add_string("Float");
    Void        = idtable.add_string("Void");  
    // Main function
    Main        = idtable.add_string("main");

    // classical function to print things, so defined here for call.
    print        = idtable.add_string("printf");
}

/*
    TODO :
    you should fill the following function defines, so that semant() can realize a semantic 
    analysis in a recursive way. 
    Of course, you can add any other functions to help.
*/

static bool sameType(Symbol name1, Symbol name2) {
    return strcmp(name1->get_string(), name2->get_string()) == 0;
}

static void install_calls(Decls decls) {
    for(int i = decls->first(); decls->more(i); i=decls->next(i))
    {
        Symbol name, type;
        name = decls-> nth(i) -> getName();
        type = decls-> nth(i) -> getType();
        if (decls-> nth(i)-> isCallDecl()){
            if (calldecl_list[name] != NULL)
            {
            semant_error(decls->nth(i)) << "Function "<<name<<" was previously defined."<<endl;
            }
            else if (type != Int && type != Float && type != String && type != Void && type != Bool )
            {
            semant_error(decls->nth(i)) << "Function type errors."<<endl;
            }
            else if (!isValidCallName(name))
            {
            semant_error(decls->nth(i)) << "Function printf cannot be redefined."<<endl;
            } 
            calldecl_list [name] = type;
            decls-> nth(i) ->checkp();  
            //cout<<name<<' '<<type<<endl;
        }
     /*for (std::map<Symbol, Symbol>::const_iterator it = calldecl_list.begin(); it != calldecl_list.end(); ++it)
      cout << it->second<<endl;*/
    }

}

static void install_globalVars(Decls decls) {
    for(int i = decls->first(); decls->more(i); i=decls->next(i)){
        Symbol name, type;
        name = decls-> nth(i) -> getName();
        type = decls-> nth(i) -> getType();
        if (!decls->nth(i)->isCallDecl()){
            if(globalvar_list[name] != NULL ){
                semant_error(decls->nth(i)) << "Global variable" <<name << "cannot be redefined."<<endl;
            }
            else if (type != Int && type != Float && type != String &&  type != Bool){
               semant_error(decls->nth(i)) << "Global variable type errors."<<endl; 
            }
        }
        globalvar_list [name] = type;
    }

}

static void check_calls(Decls decls) {
    //objectEnv.enterscope();
    for(int i = decls->first(); decls->more(i); i=decls->next(i)){
            if (decls->nth(i)->isCallDecl()) {
            //cout<<decls->nth(i)->getName()<<endl;
            decls->nth(i)->check();
        }
    }
    //objectEnv.exitscope();
}

static void check_main() {
    if (calldecl_list[Main] == NULL){
        semant_error() <<"Main function has not been defined."<<endl;
    }

}

void VariableDecl_class::check() {
    Symbol name = this ->getName();
    Symbol type = this ->getType();
    //cout<< name <<' '<<type<<endl;
    if(type == Void){
        semant_error(this)<<"Variable "<<name<<"'s cannot be void."<<endl;
    }
    if(local[num][name]/*objectEnv.lookup(name)*/!=NULL && para_list[name]==NULL) {
       semant_error(this)<<"Variable "<<name<<" cannot be redefined."<<endl; 
    }
    else{
        objectEnv.addid(name,&type);
        local[num][name]=type;
       // cout<<*objectEnv.lookup(name)<<endl;
    }
}

void CallDecl_class::checkp(){
    Variables varss = this->getVariables();
    Symbol funcName1 = this->getName();
        for (int j=varss->first(); varss->more(j); j=varss->next(j)){
         Symbol sname = varss->nth(j)->getName();
         Symbol type = varss->nth(j)->getType();
         /*if(para_list[sname] != NULL ){
                semant_error(this) << "Paramater " <<name << " cannot be redefined."<<endl;
            }*/
         //local[sname]=type;
        // para_list[sname] = type;
         para.push_back(type);
    }
    funpara [funcName1] = para;
    //cout<< funcName1<<' '<<funpara [funcName1].size()<<endl;
    if (para.size() >6){
        semant_error(this)<< "The number of paramaters cannot be more than 6."<<endl;
    }
    para.clear();

}

void CallDecl_class::check() {
    Variables vars = this->getVariables();
    Symbol funcName = this->getName(); 
    Symbol returnType = this->getType();
    StmtBlock stmtblock = this->getBody();
   // cout<<funcName<<endl;

    objectEnv.enterscope();
    local.push_back(pt);
    for (int j=vars->first(); vars->more(j); j=vars->next(j)){
         Symbol sname = vars->nth(j)->getName();
         Symbol type = vars->nth(j)->getType();
         if(type == Void ){
                semant_error(this) << "Paramater " <<sname << " cannot be void."<<endl;
            }
         if(para_list[sname] != NULL ){
                semant_error(this) << "Paramater " <<sname << " cannot be redefined."<<endl;
            }
         //objectEnv.addid(sname, &type);
         else { local[num][sname]=type;
         para_list[sname] = type;}
         //para.push_back(type); 
    }
    //funpara [funcName] = para;
    //cout<<funcName<<' '<<funpara[funcName].size()<<endl;
    para.clear();
    if (funcName == Main && vars->len() != 0) {
        semant_error(this)<<"Main function should not have paras"<<endl;
    } else if ((calldecl_list[Main] != Void) && (calldecl_list[Main] != NULL)&&(funcName == Main) ) {
        semant_error(this)<<"Main function should have return type Void."<<endl;
    }
    //cout<<funcName<<' '<<stmtblock->hasbreak()<<endl;
    if (!stmtblock ->hasreturn()){
      semant_error(this)<<"Function "<<funcName<<" must have an overall return statement."<<endl;  
    }
    stmtblock -> check(returnType);
    stmtblock->errorbreak();
    stmtblock->errorcontinue();
    objectEnv.exitscope();
    local.clear();
    num = 0;
    para_list.clear();
}

void StmtBlock_class::check(Symbol type) {
    num++;
    local.push_back(pt);
    VariableDecls varDecls = this->getVariableDecls();
    for (int j=varDecls->first(); varDecls->more(j); j=varDecls->next(j)) {
    varDecls->nth(j)->check();
    //cout<<varDecls->nth(j)->getName()<<' '<<*objectEnv.lookup(varDecls->nth(j)->getName())<<endl;
    }
    Stmts stmts = this->getStmts(); 
    for (int j=stmts->first(); stmts->more(j); j=stmts->next(j)) {
        stmts->nth(j)->check(type);
    }
    local[num].clear();
    num--;
}

void StmtBlock_class::errorbreak(){
    Stmts stmts = this->getStmts();
        for (int j=stmts->first(); stmts->more(j); j=stmts->next(j)) {
        stmts->nth(j)->errorbreak();
    }
}

void StmtBlock_class::errorcontinue(){
    Stmts stmts = this->getStmts();
        for (int j=stmts->first(); stmts->more(j); j=stmts->next(j)) {
        stmts->nth(j)->errorcontinue();
    }
}

void IfStmt_class::check(Symbol type) {
    Expr condition = this->getCondition();
    StmtBlock thenst = this->getThen();
    StmtBlock elsest = this->getElse();
    
    // If condition should be Bool
    Symbol conditionType = condition->checkType();
    if (conditionType != Bool) {
        semant_error(this)<<"condition type should be Bool, but found"<<conditionType<<endl;
    }

    // check thenExpr and elseExpr
    thenst->check(type);
    elsest->check(type);
}

void IfStmt_class::errorbreak(){
    StmtBlock thenst = this->getThen();
    StmtBlock elsest = this->getElse();

    thenst->errorbreak();
    elsest->errorbreak();

}

void IfStmt_class::errorcontinue(){
    StmtBlock thenst = this->getThen();
    StmtBlock elsest = this->getElse();

    thenst->errorcontinue();
    elsest->errorcontinue();

}

void WhileStmt_class::check(Symbol type) {
    Expr condition = this->getCondition();
    StmtBlock bodyst = this->getBody();

    Symbol conditionType = condition->checkType();
    if (conditionType != Bool) {
        semant_error(this)<<"condition type should be Bool, but found"<<conditionType<<endl;
    }

    // check while body
    bodyst->check(type);

}

void ForStmt_class::check(Symbol type) {
    Expr init = this->getInit();
    Expr condition = this->getCondition();
    Expr loop = this->getLoop();
    StmtBlock body = this->getBody();

    init->checkType();
    loop->checkType();
    // For condition should be Bool
    Symbol conditionType = condition->checkType();
    if (conditionType != Bool && !condition ->is_empty_Expr()) {
        semant_error(this)<<"condition type should be Bool, but found"<<conditionType<<endl;
    }

    // check For body
    body->check(type);

}

void ReturnStmt_class::check(Symbol type) {
    Expr expr = this->getValue();

    Symbol returnType = expr->checkType();
    //cout<<returnType<<' '<<type<<endl;
    if(expr->is_empty_Expr() && type != Void){
        semant_error(this)<<"return types error ."<<endl;
    }
    else if (!expr->is_empty_Expr() && returnType != type){
        semant_error(this)<<"return types error ."<<endl;
    }

}

void ContinueStmt_class::check(Symbol type) {

}

void ContinueStmt_class::errorcontinue(){
    semant_error(this)<<"continue must be used in a loop sentence."<<endl;
}

void BreakStmt_class::check(Symbol type) {

}

void BreakStmt_class::errorbreak(){
    semant_error(this)<<"break must be used in a loop sentence."<<endl;
}

Symbol Call_class::checkType(){
    Symbol name = this->getName();
    Actuals actuals = this->getActuals();
    int temp =0;

    if (name == print){
        if(actuals->len() == 0){
            semant_error(this)<<"Function printf paramater cannot be null"<<endl;
            return Void;
        }
        Symbol first = actuals -> nth(actuals->first()) ->checkType();
        if(first != String){
            semant_error(this)<<"Function printf first paramater must be String"<<endl;
        }
        for (int t = actuals->next( actuals->first()); actuals->more(t) ; t=actuals->next(t)){
        actuals->nth(t)->checkType();}
        this->setType(Void);
        return Void;
    }
    if (calldecl_list[name] == NULL){
         semant_error(this)<<"Function "<<name<<" has not been defined before."<<endl;
        this->setType(Void);
        return Void;
    }
    if (actuals->len() !=int(funpara[name].size())){
        semant_error(this)<<"Function "<<name<<" should have "<<funpara[name].size()<<" paramaters."<<endl;
    }
    else{
        for (int t = actuals->first(); actuals->more(t) ; t=actuals->next(t)){
        Symbol ptype = actuals->nth(t)->checkType();
        //cout<<ptype<<endl;
        //cout<<funpara[name][temp]<<endl;
        if (ptype != funpara[name][temp]){
            semant_error(this) << "Function "<<name<<" the "<<temp<<" parameter should be"<<funpara[name][temp]<<" but provided a "<<ptype<<endl;
        }
        temp ++;
        //actuals->nth(i)->checkType();
        }
    }
    this ->setType(calldecl_list[name]);
    return calldecl_list[name];

}

Symbol Actual_class::checkType(){
    this ->setType(expr->checkType());
    return type;

}

Symbol Assign_class::checkType(){
    bool flag = false;
    int k = num;
    while(k>=0){
        if(local[k][lvalue] != NULL)
        {flag = true;
        break;}
        k--;
    }
    /*for ( k = 0;k<=num;k++){
        if(local[k][lvalue] != NULL)
        {flag = true;
        break;}
    }*/
    if(!flag/*objectEnv.lookup(lvalue) == NULL*/ && globalvar_list[lvalue]==NULL){
        semant_error(this)<< "Variable "<<lvalue<<" has not been declared."<<endl;
    }
    Symbol stype = value->checkType();
    Symbol ptype = flag ? local[k][lvalue] : globalvar_list[lvalue];
    //cout<<*objectEnv.lookup(lvalue);
    //cout<<local[lvalue]<<' '<<value->checkType()<<endl;
    if (ptype/**stype*/ != stype){
        semant_error(this)<<"assign value mismatch"<<endl;
    }
    this ->setType(stype);
    return type;

}

Symbol Add_class::checkType(){
    Symbol lty = e1->checkType();
    Symbol rty = e2->checkType();
    if((lty!= Int && lty !=Float)||(rty != Float && rty != Int)){
        semant_error(this) <<"Types between + do not match."<<endl;
    }
    if((lty == Int && rty == Float) || (lty == Float && rty == Int)){
        this ->setType(Float);
        return type;
    }
    this ->setType(lty);
    return type;
}

Symbol Minus_class::checkType(){
    Symbol lty = e1->checkType();
    Symbol rty = e2->checkType();
    if((lty!= Int && lty !=Float)||(rty != Float && rty != Int)){
        semant_error(this) <<"Types between - do not match."<<endl;
    }
    if((lty == Int && rty == Float) || (lty == Float && rty == Int)){
        this ->setType(Float);
        return type;
    }
    this ->setType(lty);
    return type;

 
}

Symbol Multi_class::checkType(){
    Symbol lty = e1->checkType();
    Symbol rty = e2->checkType();
     if((lty!= Int && lty !=Float)||(rty != Float && rty != Int)){
        semant_error(this) <<"Types between * do not match."<<endl;
    }
    if((lty == Int && rty == Float) || (lty == Float && rty == Int)){
        this ->setType(Float);
        return type;
    }
    this ->setType(lty);
    return type;
 
}

Symbol Divide_class::checkType(){
    Symbol lty = e1->checkType();
    Symbol rty = e2->checkType();
     if((lty!= Int && lty !=Float)||(rty != Float && rty != Int)){
        semant_error(this) <<"Types between // do not match."<<endl;
    }
    if((lty == Int && rty == Float) || (lty == Float && rty == Int)){
        this ->setType(Float);
        return type;
    }
    this ->setType(lty);
    return type;

}

Symbol Mod_class::checkType(){
    Symbol lty = e1->checkType();
    Symbol rty = e2->checkType();
    if((lty != Int || rty != Int)){
        semant_error(this) <<"Types between % do not match."<<endl;
    }
    this ->setType(lty);
    return type;

}

Symbol Neg_class::checkType(){
    Symbol lty = e1->checkType();
    if(lty != Int ||lty != Float){
        semant_error(this) <<"Type does not match."<<endl;
    }
    this ->setType(lty);
    return type;

}

Symbol Lt_class::checkType(){
    Symbol lty = e1->checkType();
    Symbol rty = e2->checkType();
    if((lty!= Int && lty !=Float)||(rty != Float && rty != Int)){
        semant_error(this) <<"Types between < do not match."<<endl;
    }
    this ->setType(Bool);
    return type;

}

Symbol Le_class::checkType(){
    Symbol lty = e1->checkType();
    Symbol rty = e2->checkType();
    if((lty!= Int && lty !=Float)||(rty != Float && rty != Int)){
        semant_error(this) <<"Types between <= do not match."<<endl;
    }
    this ->setType(Bool);
    return type;

}

Symbol Equ_class::checkType(){
    Symbol lty = e1->checkType();
    Symbol rty = e2->checkType();
    if (((lty != Int && lty !=Float) || (rty != Int && rty != Float))&&(lty != Bool || rty != Bool)) {
        semant_error(this) <<"Types between == do not match."<<endl;
    }
    this ->setType(Bool);
    return type;

}

Symbol Neq_class::checkType(){
    Symbol lty = e1->checkType();
    Symbol rty = e2->checkType();
    if (((lty != Int && lty !=Float) || (rty != Int && rty != Float))&&(lty != Bool || rty != Bool)){
        semant_error(this) <<"Types between != do not match."<<endl;
    }
    this ->setType(Bool);
    return type;

}

Symbol Ge_class::checkType(){
    Symbol lty = e1->checkType();
    Symbol rty = e2->checkType();
    if((lty!= Int && lty !=Float)||(rty != Float && rty != Int)){
        semant_error(this) <<"Types between >= do not match."<<endl;
    }
    this ->setType(Bool);
    return type;

}

Symbol Gt_class::checkType(){
    Symbol lty = e1->checkType();
    Symbol rty = e2->checkType();
    if((lty!= Int && lty !=Float)||(rty != Float && rty != Int)){
        semant_error(this) <<"Types between > do not match."<<endl;
    }
    this ->setType(Bool);
    return type;

}

Symbol And_class::checkType(){
    Symbol lty = e1->checkType();
    Symbol rty = e2->checkType();
    if(lty!=Bool || rty!=Bool){
        semant_error(this) <<"Types between && do not match."<<endl;
    }
    this ->setType(Bool);
    return type;

}

Symbol Or_class::checkType(){
    Symbol lty = e1->checkType();
    Symbol rty = e2->checkType();
    if(lty!=Bool || rty!=Bool){
        semant_error(this) <<"Types between || do not match."<<endl;
    }
    this ->setType(Bool);
    return type;

}

Symbol Xor_class::checkType(){
    Symbol lty = e1->checkType();
    Symbol rty = e2->checkType();
    if(lty!=Bool || rty!=Bool){
        semant_error(this) <<"Types between ^ do not match."<<endl;
    }
    this ->setType(Bool);
    return type;

}

Symbol Not_class::checkType(){
    Symbol lty = e1->checkType();
    if(lty!=Bool ){
        semant_error(this) <<"Types between ! do not match."<<endl;
    }
    this ->setType(Bool);
    return type;

}

Symbol Bitand_class::checkType(){
    Symbol lty = e1->checkType();
    Symbol rty = e2->checkType();
    if(lty!=Int || rty!=Int){
        semant_error(this) <<"Types between & do not match."<<endl;
    }
    this ->setType(Int);
    return type;

}

Symbol Bitor_class::checkType(){
    Symbol lty = e1->checkType();
    Symbol rty = e2->checkType();
    if(lty!=Int || rty!=Int){
        semant_error(this) <<"Types between | do not match."<<endl;
    }
    this ->setType(Int);
    return type;

}

Symbol Bitnot_class::checkType(){
    Symbol lty = e1->checkType();
    if(lty!=Int ){
        semant_error(this) <<"Types between ~ do not match."<<endl;
    }
    this ->setType(Int);
    return type;

}

Symbol Const_int_class::checkType(){
    setType(Int);
    return type;
}

Symbol Const_string_class::checkType(){
    setType(String);
    return type;
}

Symbol Const_float_class::checkType(){
    setType(Float);
    return type;
}

Symbol Const_bool_class::checkType(){
    setType(Bool);
    return type;
}

Symbol Object_class::checkType(){
    bool flag = false;
    int k = num;
    while(k>=0){
        if(local[k][var] != NULL)
        {flag = true;
        break;}
        k--;
    }
    
    if (!flag/*objectEnv.lookup(var)*/  && globalvar_list[var]==NULL) {
    semant_error(this)<<"object "<<var<<" has not been defined."<<endl;
    this->setType(Void);
    return type;
    }
    Symbol* ty =objectEnv.probe(var);
    Symbol ptype = flag ? local[k][var] : globalvar_list[var];
    this->setType(ptype);
    return type;

}

Symbol No_expr_class::checkType(){
    setType(Void);
    return getType();
}

void Program_class::semant() {
    initialize_constants();
    install_calls(decls);
    check_main();
    install_globalVars(decls);
    check_calls(decls);
    
    if (semant_errors > 0) {
        cerr << "Compilation halted due to static semantic errors." << endl;
        exit(1);
    }
}



