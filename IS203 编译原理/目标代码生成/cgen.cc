
//**************************************************************
//
// Code generator SKELETON
//
//
//**************************************************************

#include "cgen.h"
#include "cgen_gc.h"
#include <vector>

using namespace std;

extern void emit_string_constant(ostream& str, char *s);
extern int cgen_debug;

typedef SymbolTable<Symbol, int> variableTable;
variableTable variabletable;

static char *CALL_REGS[] = {RDI, RSI, RDX, RCX, R8, R9};
static char *CALL_XMM[] = {XMM0, XMM1, XMM2, XMM3};

int offset = 0;
int tempadd = 0;
int pos = 0;
int conpos = 0;
int breakpos = 0;
char *gol[6] = {new char, new char, new char, new char, new char, new char};
bool flag[6] = {0, 0, 0, 0, 0, 0};
int para = 0;
bool fun = 0;
bool flag1 = 0;
char *goo = new char;

void cgen_helper(Decls decls, ostream& s);
void code(Decls decls, ostream& s);

//////////////////////////////////////////////////////////////////
//
//
//    Helper Functions
//  
//
//////////////////////////////////////////////////////////////////

// you can add any helper functions here


//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
Symbol 
    Int,
    Float,
    String,
    Bool,
    Void,
    Main,
    print
    ;
//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
    // 4 basic types and Void type
    Bool        = idtable.add_string("Bool");
    Int         = idtable.add_string("Int");
    String      = idtable.add_string("String");
    Float       = idtable.add_string("Float");
    Void        = idtable.add_string("Void");  
    // main function
    Main        = idtable.add_string("main");

    // classical function to print things, so defined here for call.
    print        = idtable.add_string("printf");
}


//*********************************************************
//
// Define method for code generation
//
//
//*********************************************************

void Program_class::cgen(ostream &os) 
{
  // spim wants comments to start with '#'
  os << "# start of generated code\n";

  initialize_constants();
  cgen_helper(decls,os);

  os << "\n# end of generated code\n";
}


//////////////////////////////////////////////////////////////////////////////
//
//  emit_* procedures
//
//  emit_X  writes code for operation "X" to the output stream.
//  There is an emit_X for each opcode X, as well as emit_ functions
//  for generating names according to the naming conventions (see emit.h)
//  and calls to support functions defined in the trap handler.
//
//  Register names and addresses are passed as strings.  See `emit.h'
//  for symbolic names you can use to refer to the strings.
//
//////////////////////////////////////////////////////////////////////////////

static void emit_mov(const char *source, const char *dest, ostream& s)
{
  s << MOV << source << COMMA << dest << endl;
}

static void emit_rmmov(const char *source_reg, int offset, const char *base_reg, ostream& s)
{
  s << MOV << source_reg << COMMA << offset << "(" << base_reg << ")"
      << endl;
}

static void emit_mrmov(const char *base_reg, int offset, const char *dest_reg, ostream& s)
{
  s << MOV << offset << "(" << base_reg << ")" << COMMA << dest_reg  
      << endl;
}

static void emit_irmov(const char *immidiate, const char *dest_reg, ostream& s)
{
  s << MOV << "$" << immidiate << COMMA << dest_reg  
      << endl;
}

static void emit_irmovl(const char *immidiate, const char *dest_reg, ostream& s)
{
  s << MOVL << "$" << immidiate << COMMA << dest_reg  
      << endl;
}

static void emit_immov(const char *immidiate, int offset, const char *base_reg, ostream& s)
{
  s << MOV << "$" << immidiate << COMMA << "(" << offset << ")" << base_reg  
      << endl;
}

static void emit_add(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << ADD << source_reg << COMMA << dest_reg << endl;
}

static void emit_sub(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << SUB << source_reg << COMMA << dest_reg << endl;
}

static void emit_mul(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << MUL << source_reg << COMMA << dest_reg << endl;
}

static void emit_div(const char *dest_reg, ostream& s)
{
  s << DIV << dest_reg << endl;
}

static void emit_cqto(ostream &s)
{
  s << CQTO << endl;
}

static void emit_neg(const char *dest_reg, ostream& s)
{
  s << NEG << dest_reg << endl;
}

static void emit_and(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << AND << source_reg << COMMA << dest_reg << endl;
}

static void emit_or(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << OR << source_reg << COMMA << dest_reg << endl;
}

static void emit_xor(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << XOR << source_reg << COMMA << dest_reg << endl;
}

static void emit_not(const char *dest_reg, ostream& s)
{
  s << NOT << " " << dest_reg << endl;
}

static void emit_movsd(const char *source, const char *dest, ostream& s)
{
  s << MOVSD << source << COMMA << dest << endl;
}

static void emit_movaps(const char *source, const char *dest, ostream& s)
{
  s << MOVAPS << source << COMMA << dest << endl;
}

static void emit_addsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << ADDSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_subsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << SUBSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_mulsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << MULSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_divsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << DIVSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_cmp(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << CMP << source_reg << COMMA << dest_reg << endl;
}

static void emit_test(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << TEST << source_reg << COMMA << dest_reg << endl;
}

static void emit_ucompisd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << UCOMPISD << source_reg << COMMA << dest_reg << endl;
}

static void emit_xorpd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << XORPD << source_reg << COMMA << dest_reg << endl;
}
static void emit_jmp(const char *dest, ostream& s)
{
  s << JMP << " " << dest << endl;
}

static void emit_jl(const char *dest, ostream& s)
{
  s << JL << " " << dest << endl;
}

static void emit_jle(const char *dest, ostream& s)
{
  s << JLE << " " << dest << endl;
}

static void emit_je(const char *dest, ostream& s)
{
  s << JE << " " << dest << endl;
}

static void emit_jne(const char *dest, ostream& s)
{
  s << JNE << " " << dest << endl;
}

static void emit_jg(const char *dest, ostream& s)
{
  s << JG << " " << dest << endl;
}

static void emit_jge(const char *dest, ostream& s)
{
  s << JGE << " " << dest << endl;
}

static void emit_jb(const char *dest, ostream& s)
{
  s << JB << " " << dest << endl;
}

static void emit_jbe(const char *dest, ostream& s)
{
  s << JBE << " " << dest << endl;
}

static void emit_ja(const char *dest, ostream& s)
{
  s << JA << " " << dest << endl;
}

static void emit_jae(const char *dest, ostream& s)
{
  s << JAE << " " << dest << endl;
}

static void emit_jp(const char *dest, ostream& s)
{
  s << JP << " " << dest << endl;
}

static void emit_jz(const char *dest, ostream& s)
{
  s << JZ << " " << dest << endl;
}

static void emit_jnz(const char *dest, ostream& s)
{
  s << JNZ << " " << dest << endl;
}

static void emit_call(const char *dest, ostream& s)
{
  s << CALL << " " << dest << endl;
}

static void emit_ret(ostream& s)
{
  s << RET << endl;
}

static void emit_push(const char *reg, ostream& s)
{
  s << PUSH << " " << reg << endl;
}

static void emit_pop(const char *reg, ostream& s)
{
  s << POP << " " << reg << endl;
}

static void emit_leave(ostream& s)
{
  s << LEAVE << endl;
}

static void emit_position(const char *p, ostream& s)
{
  s << p << ":" << endl;
}

static void emit_float_to_int(const char *float_mmx, const char *int_reg, ostream& s)
{
  s << CVTTSD2SIQ << float_mmx << COMMA << int_reg << endl;
}

static void emit_int_to_float(const char *int_reg, const char *float_mmx, ostream& s)
{
  s << CVTSI2SDQ << int_reg << COMMA << float_mmx << endl;
}
///////////////////////////////////////////////////////////////////////////////
//
// coding strings, ints, and booleans
//
// Seal has four kinds of constants: strings, ints, and booleans.
// This section defines code generation for each type.
//
// If you like, you can add any ***Entry::code_def() and ***Entry::code_ref()
// functions to help.
//
///////////////////////////////////////////////////////////////////////////////

//
// Strings
//
void StringEntry::code_ref(ostream& s)
{
  s << "$" << STRINGCONST_PREFIX << index;
}

//
// Emit code for a constant String.
//

void StringEntry::code_def(ostream& s)
{
  s << STRINGCONST_PREFIX << index << ":" << endl;
  s  << STRINGTAG ; emit_string_constant(s,str);                                                // align to word
}

//
// StrTable::code_string
// Generate a string object definition for every string constant in the 
// stringtable.
//
void StrTable::code_string_table(ostream& s)
{  
  for (List<StringEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s);
}

// the following 2 functions are useless, please DO NOT care about them
void FloatEntry::code_ref(ostream &s)
{
  s << FLOATTAG << index;
}

void IntEntry::code_def(ostream &s)
{
  s << GLOBAL;
}

//***************************************************
//
//  Emit global var and functions.
//
//***************************************************

static void emit_global_int(Symbol name, ostream& s) {
  s << GLOBAL << name << endl << 
  ALIGN << 8 << endl << 
  SYMBOL_TYPE << name << COMMA << OBJECT << endl <<
  SIZE << name << COMMA << 8 << endl << 
  name << ":" << endl << 
  INTTAG << 0 << endl;
}

static void emit_global_float(Symbol name, ostream& s) {
  s << GLOBAL << name << endl << 
  ALIGN << 8 << endl << 
  SYMBOL_TYPE << name << COMMA << OBJECT << endl <<
  SIZE << name << COMMA << 8 << endl << 
  name << ":" << endl <<
  FLOATTAG << 0 << endl <<
  FLOATTAG << 0 << endl;
}

static void emit_global_bool(Symbol name, ostream& s) {
  s << GLOBAL << name << endl << 
  ALIGN << 8 << endl << 
  SYMBOL_TYPE << name << COMMA << OBJECT << endl <<
  SIZE << name << COMMA << 8 << endl << 
  name << ":" << endl << 
  BOOLTAG << 0 << endl;
}

void code_global_data(Decls decls, ostream &str)
{
  int num = 0;
   for (int i=decls->first(); decls->more(i); i=decls->next(i)){
     if (!decls->nth(i)->isCallDecl()){
       if (num == 0) str << DATA << endl;
       num++;
       Symbol name = decls -> nth(i) -> getName();
       Symbol type = decls -> nth(i) -> getType();
       if (type == Int){
         emit_global_int(name, str);
       }
       else if (type == Float )
       {
         emit_global_float(name, str);
       }
       else if (type == Bool)
       {
         emit_global_bool(name, str);
       }
     }
   }
}

void code_calls(Decls decls, ostream &str) {
  str<<SECTION<<RODATA<<endl;

  stringtable.code_string_table(str);

  str<<TEXT<<endl;
  for (int i=decls->first(); decls->more(i); i=decls->next(i)) {
  if (decls->nth(i)->isCallDecl()) {
    if (decls->nth(i)->getName() == Main) {
      offset = tempadd = 0;
    } else {
      offset = tempadd = -56;
    }
    decls->nth(i)->code(str);
  }
  }

}

//***************************************************
//
//  Emit code to start the .text segment and to
//  declare the global names.
//
//***************************************************



//********************************************************
//
// Cgen helper helps to initialize and call code() function.
// You can do any initializing operations here
//
//********************************************************

void cgen_helper(Decls decls, ostream& s)
{

  code(decls, s);
}


void code(Decls decls, ostream& s)
{
  if (cgen_debug) cout << "Coding global data" << endl;
  code_global_data(decls, s);

  if (cgen_debug) cout << "Coding calls" << endl;
  code_calls(decls, s);
}

//******************************************************************
//
//   Fill in the following methods to produce code for the
//   appropriate expression.  You may add or remove parameters
//   as you wish, but if you do, remember to change the parameters
//   of the declarations in `seal-decl.h', `seal-expr.h' and `seal-stmt.h'
//   Sample code for constant integers, strings, and booleans are provided.
//   
//*****************************************************************

void CallDecl_class::code(ostream &s) {
  int float_num = 0;
  int int_num = 0;
  variabletable.enterscope();

  s<<GLOBAL<<name<<endl<<
  SYMBOL_TYPE<<name<<COMMA<<FUNCTION<<endl;

  s<<name<<":"<<endl;
  emit_push(RBP, s);
  emit_mov(RSP, RBP, s);
  emit_push(RBX, s);
  emit_push(R10, s);
  emit_push(R11, s);
  emit_push(R12, s);
  emit_push(R13, s);
  emit_push(R14, s);
  emit_push(R15, s); 

  for (int i=paras->first(); paras->more(i); i=paras->next(i)){
    Symbol name = paras->nth(i)->getName();
    Symbol type = paras->nth(i)->getType();

    emit_sub("$8", RSP, s);
    offset -= 8;

    if (type == Float){
      s << MOV << CALL_XMM[float_num ++] << COMMA << offset << '(' << RBP << ')' <<endl;
    }else{
      s << MOV << CALL_REGS[int_num ++] << COMMA << offset << '(' << RBP << ')'<<endl;
    }
    variabletable.addid(name, new int(offset));
  }

  body->code(s);

  s<<SIZE<<name<<", "<<".-"<<name<<endl;
  variabletable.exitscope();
}

void StmtBlock_class::code(ostream &s){

  for (int i=vars->first(); vars->more(i); i=vars->next(i)) {
    Symbol name = vars->nth(i)->getName();

    offset -= 8;
    variabletable.addid(name, new int(offset));
    
    emit_sub("$8", RSP, s);
  }

  for (int i=stmts->first(); stmts->more(i); i=stmts->next(i)) {
    stmts->nth(i)->code(s);
  }
}

void IfStmt_class::code(ostream &s) {
  condition->code(s);
  emit_mrmov(RBP,offset,RAX,s);
  emit_test(RAX, RAX, s);
  int else_pos = pos;
  pos++;
  int then_pos = pos;
  pos++;
  s<<JZ<<" "<<POSITION<<else_pos<<endl;
  thenexpr->code(s);
  s<<JMP<<" "<<POSITION<<then_pos<<endl;
  s<<POSITION<<else_pos<<":"<<endl;
  elseexpr->code(s);
  s<<POSITION<<then_pos<<":"<<endl;
}

void WhileStmt_class::code(ostream &s) {
  int start_pos = pos;
  conpos = pos;
  pos++;
  int end_pos = pos;
  breakpos = pos;
  pos++;

  s<<POSITION<<start_pos<<":"<<endl;
  condition->code(s);
  emit_mrmov(RBP,offset,RAX,s);
  emit_test(RAX, RAX, s);
  s<<JZ<<' '<<POSITION<<end_pos<<endl;
  body->code(s);
  s<<JMP<<' '<<POSITION<<start_pos<<endl;
  s<<POSITION<<end_pos<<":"<<endl;
}

void ForStmt_class::code(ostream &s) {
  initexpr->code(s);
  int start_pos = pos;
  pos++;

  int loopactpos = pos;
  conpos = pos;
  pos++;
  
  s<<POSITION<<start_pos<<":"<<endl;
  condition->code(s);
  emit_mrmov(RBP,offset,RAX,s);
  emit_test(RAX, RAX, s);
  int end_pos = pos;
  breakpos = pos;
  pos++;
  s<<JZ<<' '<<POSITION<<end_pos<<endl;
  body -> code(s);
  s<<POSITION<<loopactpos<<":"<<endl;
  loopact->code(s);
  s<<JMP<<' '<<POSITION<<start_pos<<endl;
  s<<POSITION<<end_pos<<":"<<endl;
}

void ReturnStmt_class::code(ostream &s) {
  value -> code(s);
  
  Symbol rtype = value ->getType();
  if (rtype == Float){
    s << MOVAPS << tempadd <<"("<<RBP<<")"<<COMMA<<XMM0<<endl;
  }
  if (rtype == Int || rtype == String || rtype == Bool){
    emit_mrmov(RBP,tempadd,RAX,s);
  }

  emit_pop(R15, s);
  emit_pop(R14, s);
  emit_pop(R13, s);
  emit_pop(R12, s);
  emit_pop(R11, s);
  emit_pop(R10, s);
  emit_pop(RBX, s);

  s<<LEAVE<<endl
  <<RET<<endl;
  
}

void ContinueStmt_class::code(ostream &s) {
  s<<JMP<<' '<<POSITION<<conpos<<endl;
 
}

void BreakStmt_class::code(ostream &s) {
  s<<JMP<<' '<<POSITION<<breakpos<<endl;
}

void Call_class::code(ostream &s) {
  fun = 1;
  int int_num = 0;
  int float_num = 0;
  int addr[actuals->len()];
  int num = 0;
  int tt = 0;

  for (int i=actuals->first(); actuals->more(i); i=actuals->next(i)) {
    if (actuals->nth(i)->getType()->get_string() == Int->get_string() || actuals->nth(i)->getType()->get_string() == Bool->get_string() || actuals->nth(i)->getType()->get_string() == String->get_string()) {
      actuals->nth(i)->code(s);
      addr[i] = tempadd;
      para++;
    }

    if (actuals->nth(i)->getType()->get_string() == Float->get_string()) {
      num ++;
      actuals->nth(i)->code(s);
      addr[i] = tempadd;
      para++;
    }
  }

  for (int i=actuals->first(); actuals->more(i); i=actuals->next(i)) {
    if (actuals->nth(i)->getType()->get_string() == Int->get_string() || actuals->nth(i)->getType()->get_string() == Bool->get_string() || actuals->nth(i)->getType()->get_string() == String->get_string()) {
      if (!flag[i])
      {s<<MOV<<addr[i]<<"("<<RBP<<")"<<COMMA<<CALL_REGS[int_num ++]<<endl;}
      else{
      s<<MOV<<gol[i]<<"("<<RIP<<")"<<COMMA<<CALL_REGS[int_num ++]<<endl;
      }
    } else if (actuals->nth(i)->getType()->get_string() == Float->get_string()) {
      if(!flag[i])
      {s<<MOVSD<<addr[i]<<"("<<RBP<<")"<<COMMA<<CALL_XMM[float_num ++]<<endl;}
      else{
        s<<MOVSD<<gol[i]<<"("<<RIP<<")"<<COMMA<<CALL_XMM[float_num ++]<<endl;
      }
    }
  }

  if (name == print) {
    emit_sub("$8", RSP, s);
    offset -= 8;
    std::string c;
    c = std::to_string(num);
    const char* c_s = c.c_str();
    emit_irmovl(c_s, EAX, s);
    emit_call("printf", s);
  }
  if (type == Int || type == Bool || type == String ){
    emit_call(name-> get_string() ,s);
    emit_sub("$8",RSP,s);
    offset -= 8;
    tempadd = offset;
    emit_rmmov(RAX,offset,RBP,s);
  }
    if (type == Float ){
    emit_call(name-> get_string() ,s);
    emit_sub("$8",RSP,s);
    offset -= 8;
    tempadd = offset;
    s << MOVSD << XMM0 << COMMA << offset << "(" << RBP<< ")" << endl;
  }

  fun = 0;
  para = 0;
  for (int t =0; t<6;t++){
    strcpy(gol[t],new char);
    flag[t]=0;
  }
  
}

void Actual_class::code(ostream &s) {
  expr->code(s); 
}

void Assign_class::code(ostream &s) {
  value->code(s);
  int t = 0;

  emit_mrmov(RBP, tempadd, RAX, s);
  if(variabletable.lookup(lvalue)!=NULL)
  {t = *variabletable.lookup(lvalue);
  emit_rmmov(RAX, t, RBP, s);}
  else{
    s << MOV << RAX << COMMA << lvalue ->get_string() << "(" << RIP << ")"<< endl;
  }
}

void Add_class::code(ostream &s) {
  e1 -> code(s);
  int e1add = tempadd;
  bool a1 = flag1;
  char *b1 = goo;
  flag1 = 0;
  e2 -> code(s);
  int e2add = tempadd;
  bool a2 = flag1;
  char *b2 = goo;
  flag1 = 0;
  offset -=8;
  tempadd = offset;
  emit_sub("$8", RSP, s);

  if (e1->getType()->get_string() == Int->get_string() && e2->getType()->get_string() == Int->get_string()){
    if(!a1)
    {emit_mrmov(RBP ,e1add, RBX,s);}
    else{
     s << MOV <<  b1 << "(" << RIP << ")"<< COMMA << RBX <<endl; 
    }
    if(!a2)
    {emit_mrmov(RBP ,e2add, R10,s);}
    else{
     s << MOV <<  b2 << "(" << RIP << ")"<< COMMA << R10 <<endl; 
    }
    emit_add(RBX,R10,s);
    emit_rmmov(R10,offset,RBP,s);
  } 
  if (e1->getType()->get_string() == Float->get_string() && e2->getType()->get_string() == Float->get_string()){
    if(!a1)
    {s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM4 << endl;}
    else{
     s << MOVSD << b1 << "(" << RIP << ")" << COMMA << XMM4 << endl;
    }
    if (!a2)
    {s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM5 << endl;}
    else{
     s << MOVSD << b2 << "(" << RIP << ")" << COMMA << XMM5 << endl;
    }
    emit_addsd(XMM4,XMM5,s);
    s << MOVSD << XMM5 << COMMA << offset << "(" << RBP<< ")" << endl;
  }
  if (e1->getType()->get_string() == Float->get_string() && e2->getType()->get_string() == Int->get_string()){
    if(!a1)
    {s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM4 << endl;}
    else{
      s << MOVSD << b1 << "(" << RIP << ")" << COMMA << XMM4 << endl;
    }
    if(!a2)
    {emit_mrmov(RBP ,e2add, RBX,s);}
    else{
     s << MOV <<  b2 << "(" << RIP << ")"<< COMMA << RBX <<endl; 
    }
    emit_int_to_float(RBX, XMM5, s);
    emit_addsd(XMM4, XMM5, s);
    s << MOVSD << XMM5 << COMMA << offset << "(" << RBP<< ")" << endl;
  }
  if (e1->getType()->get_string() == Int->get_string() && e2->getType()->get_string() == Float->get_string()){
    if (!a2)
    {s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM4 << endl;}
    else {
     s << MOVSD << b2 << "(" << RIP << ")" << COMMA << XMM4 << endl;
    }
    if(!a1)
    {emit_mrmov(RBP ,e1add, RBX,s);}
    else
    {
      s << MOV <<  b1 << "(" << RIP << ")"<< COMMA << RBX <<endl;
    }
    
    emit_int_to_float(RBX, XMM5, s);
    emit_addsd(XMM4, XMM5, s);
    s << MOVSD << XMM5 << COMMA << offset << "(" << RBP<< ")" << endl;
  }
  
}

void Minus_class::code(ostream &s) {
  e1 -> code(s);
  int e1add = tempadd;
  bool a1 = flag1;
  char *b1 = goo;
  flag1 = 0;
  e2 -> code(s);
  int e2add = tempadd;
  bool a2 = flag1;
  char *b2 = goo;
  flag1 = 0;
  offset -=8;
  tempadd = offset;
  emit_sub("$8", RSP, s);

  if (e1->getType()->get_string() == Int->get_string() && e2->getType()->get_string() == Int->get_string()){
    if(!a1)
    {emit_mrmov(RBP ,e1add, RBX,s);}
    else{
     s << MOV <<  b1 << "(" << RIP << ")"<< COMMA << RBX <<endl; 
    }
    if(!a2)
    {emit_mrmov(RBP ,e2add, R10,s);}
    else{
     s << MOV <<  b2 << "(" << RIP << ")"<< COMMA << R10 <<endl; 
    }
    emit_sub(R10,RBX,s);
    emit_rmmov(RBX,offset,RBP,s);
  } 
  if (e1->getType()->get_string() == Float->get_string() && e2->getType()->get_string() == Float->get_string()){
    if(!a1)
    {s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM4 << endl;}
    else{
     s << MOVSD << b1 << "(" << RIP << ")" << COMMA << XMM4 << endl;
    }
    if (!a2)
    {s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM5 << endl;}
    else{
     s << MOVSD << b2 << "(" << RIP << ")" << COMMA << XMM5 << endl;
    }
    emit_subsd(XMM5,XMM4,s);
    s << MOVSD << XMM4 << COMMA << offset << "(" << RBP<< ")" << endl;
  }
  if (e1->getType()->get_string() == Float->get_string() && e2->getType()->get_string() == Int->get_string()){
    if(!a1)
    {s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM4 << endl;}
    else{
      s << MOVSD << b1 << "(" << RIP << ")" << COMMA << XMM4 << endl;
    }
    if(!a2)
    {emit_mrmov(RBP ,e2add, RBX,s);}
    else{
     s << MOV <<  b2 << "(" << RIP << ")"<< COMMA << RBX <<endl; 
    }
    emit_int_to_float(RBX, XMM5, s);
    emit_subsd(XMM5, XMM4, s);
    s << MOVSD << XMM4 << COMMA << offset << "(" << RBP<< ")" << endl;
  }
    if (e1->getType()->get_string() == Int->get_string() && e2->getType()->get_string() == Float->get_string()){
    if (!a2)
    {s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM5 << endl;}
    else {
     s << MOVSD << b2 << "(" << RIP << ")" << COMMA << XMM5 << endl;
    }
    if(!a1)
    {emit_mrmov(RBP ,e1add, RBX,s);}
    else
    {
      s << MOV <<  b1 << "(" << RIP << ")"<< COMMA << RBX <<endl;
    }
    emit_int_to_float(RBX, XMM4, s);
    emit_subsd(XMM5, XMM4, s);
    s << MOVSD << XMM4 << COMMA << offset << "(" << RBP<< ")" << endl;
  }
 
}

void Multi_class::code(ostream &s) {
  e1 -> code(s);
  int e1add = tempadd;
  bool a1 = flag1;
  char *b1 = goo;
  flag1 = 0;
  e2 -> code(s);
  int e2add = tempadd;
  bool a2 = flag1;
  char *b2 = goo;
  flag1 = 0;
  offset -=8;
  tempadd = offset;
  emit_sub("$8", RSP, s);

  if (e1->getType()->get_string() == Int->get_string() && e2->getType()->get_string() == Int->get_string()){
    if(!a1)
    {emit_mrmov(RBP ,e1add, RBX,s);}
    else{
     s << MOV <<  b1 << "(" << RIP << ")"<< COMMA << RBX <<endl; 
    }
    if(!a2)
    {emit_mrmov(RBP ,e2add, R10,s);}
    else{
     s << MOV <<  b2 << "(" << RIP << ")"<< COMMA << R10 <<endl; 
    }
    emit_mul(RBX,R10,s);
    emit_rmmov(R10,offset,RBP,s);
  } 
  if (e1->getType()->get_string() == Float->get_string() && e2->getType()->get_string() == Float->get_string()){
    if(!a1)
    {s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM4 << endl;}
    else{
     s << MOVSD << b1 << "(" << RIP << ")" << COMMA << XMM4 << endl;
    }
    if (!a2)
    {s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM5 << endl;}
    else{
     s << MOVSD << b2 << "(" << RIP << ")" << COMMA << XMM5 << endl;
    }
    emit_mulsd(XMM4,XMM5,s);
    s << MOVSD << XMM5 << COMMA << offset << "(" << RBP<< ")" << endl;
  }
  if (e1->getType()->get_string() == Float->get_string() && e2->getType()->get_string() == Int->get_string()){
    if(!a1)
    {s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM4 << endl;}
    else{
      s << MOVSD << b1 << "(" << RIP << ")" << COMMA << XMM4 << endl;
    }
    if(!a2)
    {emit_mrmov(RBP ,e2add, RBX,s);}
    else{
     s << MOV <<  b2 << "(" << RIP << ")"<< COMMA << RBX <<endl; 
    }
    emit_int_to_float(RBX, XMM5, s);
    emit_mulsd(XMM4, XMM5, s);
    s << MOVSD << XMM5 << COMMA << offset << "(" << RBP<< ")" << endl;
  }
    if (e1->getType()->get_string() == Int->get_string() && e2->getType()->get_string() == Float->get_string()){
   if (!a2)
    {s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM4 << endl;}
    else {
     s << MOVSD << b2 << "(" << RIP << ")" << COMMA << XMM4 << endl;
    }
    if(!a1)
    {emit_mrmov(RBP ,e1add, RBX,s);}
    else
    {
      s << MOV <<  b1 << "(" << RIP << ")"<< COMMA << RBX <<endl;
    }
    emit_int_to_float(RBX, XMM5, s);
    emit_mulsd(XMM4, XMM5, s);
    s << MOVSD << XMM5 << COMMA << offset << "(" << RBP<< ")" << endl;
  }
 
}

void Divide_class::code(ostream &s) {
  e1 -> code(s);
  int e1add = tempadd;
  bool a1 = flag1;
  char *b1 = goo;
  flag1 = 0;
  e2 -> code(s);
  int e2add = tempadd;
  bool a2 = flag1;
  char *b2 = goo;
  flag1 = 0;
  offset -=8;
  tempadd = offset;
  emit_sub("$8", RSP, s);

  if (e1->getType()->get_string() == Int->get_string() && e2->getType()->get_string() == Int->get_string()){
    if(!a1)
    {emit_mrmov(RBP ,e1add, RAX,s);}
    else{
     s << MOV <<  b1 << "(" << RIP << ")"<< COMMA << RAX <<endl; 
    }
    emit_cqto(s);
    if(!a2)
    {emit_mrmov(RBP ,e2add, RBX,s);}
    else{
     s << MOV <<  b2 << "(" << RIP << ")"<< COMMA << RBX <<endl; 
    }
    emit_div(RBX, s);
    emit_rmmov(RAX,offset,RBP,s);
  }
  if (e1->getType()->get_string() == Float->get_string() && e2->getType()->get_string() == Float->get_string()){
    if(!a1)
    {s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM4 << endl;}
    else{
     s << MOVSD << b1 << "(" << RIP << ")" << COMMA << XMM4 << endl;
    }
    if (!a2)
    {s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM5 << endl;}
    else{
     s << MOVSD << b2 << "(" << RIP << ")" << COMMA << XMM5 << endl;
    }
    emit_divsd(XMM5, XMM4, s);
    s << MOVSD << XMM4 << COMMA << offset << "(" << RBP<< ")" << endl;
  }
  if (e1->getType()->get_string() == Float->get_string() && e2->getType()->get_string() == Int->get_string()){
    if(!a1)
    {s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM4 << endl;}
    else{
      s << MOVSD << b1 << "(" << RIP << ")" << COMMA << XMM4 << endl;
    }
    if(!a2)
    {emit_mrmov(RBP ,e2add, RBX,s);}
    else{
     s << MOV <<  b2 << "(" << RIP << ")"<< COMMA << RBX <<endl; 
    }
    emit_divsd(XMM5, XMM4, s);
    s << MOVSD << XMM4 << COMMA << offset << "(" << RBP<< ")" << endl;
  }
  if (e1->getType()->get_string() == Int ->get_string() && e2->getType()->get_string() == Float->get_string()){
    if (!a2)
    {s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM5 << endl;}
    else {
     s << MOVSD << b2 << "(" << RIP << ")" << COMMA << XMM5 << endl;
    }
    if(!a1)
    {emit_mrmov(RBP ,e1add, RBX,s);}
    else
    {
      s << MOV <<  b1 << "(" << RIP << ")"<< COMMA << RBX <<endl;
    }
    emit_int_to_float(RBX, XMM4, s);
    emit_divsd(XMM5, XMM4, s);
    s << MOVSD << XMM4 << COMMA << offset << "(" << RBP<< ")" << endl;
  }


 
}

void Mod_class::code(ostream &s) {
  e1 -> code(s);
  int e1add = tempadd;
  e2 -> code (s);
  int e2add = tempadd;
  offset -=8;
  tempadd = offset;
  emit_sub("$8", RSP, s);
  emit_mrmov(RBP ,e1add, RAX,s);
  emit_cqto(s);
  emit_mrmov(RBP ,e2add, RBX,s);
  emit_div(RBX, s);
  emit_rmmov(RDX, offset, RBP, s);
}

void Neg_class::code(ostream &s) {
  e1 -> code(s);
  int e1add = tempadd;
  offset -=8;
  tempadd = offset;
  emit_sub("$8", RSP, s);

  if(e1->getType() == Int){
    emit_mrmov(RBP ,e1add, RAX,s);
    emit_neg(RAX,s);
    emit_rmmov(RAX, offset, RBP, s);
  }
  if(e1->getType()==Float){
    emit_mov("$0x8000000000000000",RAX,s);
    emit_mrmov(RBP,offset,RDX,s);
    emit_xor(RAX,RDX,s);
    emit_rmmov(RDX, offset, RBP, s);
  }
 
}

void Lt_class::code(ostream &s) {
  e1 -> code(s);
  int e1add = tempadd;
  e2 -> code(s);
  int e2add = tempadd;
  offset -=8;
  tempadd = offset;
  emit_sub("$8", RSP, s);

  if(e1->getType()==Int && e2 ->getType()==Int){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;
    emit_mrmov(RBP ,e1add, RAX,s);
    emit_mrmov(RBP ,e2add, RDX,s);
    emit_cmp(RDX,RAX,s);
    s<<JL<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);
  }
  if(e1 -> getType() == Float && e2 -> getType() == Float){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;
    s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM1 << endl;
    s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM0 << endl;
    emit_ucompisd(XMM0, XMM1, s);
    s<<JB<<" "<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);   
  }
  if(e1 -> getType() == Float && e2 -> getType() == Int){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;   
    emit_mrmov(RBP ,e2add, RAX,s);
    emit_int_to_float(RAX, XMM0, s);
    s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM0, XMM1, s);
    s<<JB<<" "<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);  
  }
  if(e1 -> getType() == Int && e2 -> getType() == Float){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;
    emit_mrmov(RBP ,e1add, RAX,s);
    emit_int_to_float(RAX, XMM0, s); 
    s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM1, XMM0, s);
    s<<JB<<" "<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);
  } 
}

void Le_class::code(ostream &s) {
  e1 -> code(s);
  int e1add = tempadd;
  e2 -> code (s);
  int e2add = tempadd;

  emit_sub("$8", RSP, s);
  offset -= 8;
  tempadd = offset;
  if(e1->getType()==Int && e2 ->getType()==Int){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;
    emit_mrmov(RBP ,e1add, RAX,s);
    emit_mrmov(RBP ,e2add, RDX,s);
    emit_cmp(RDX,RAX,s);
    s<<JLE<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);
  }
  if(e1 -> getType() == Float && e2 -> getType() == Float){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;
    s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM1 << endl;
    s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM0 << endl;
    emit_ucompisd(XMM0, XMM1, s);
    s<<JBE<<" "<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);   
  }
  if(e1 -> getType() == Float && e2 -> getType() == Int){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;   
    emit_mrmov(RBP ,e2add, RAX,s);
    emit_int_to_float(RAX, XMM0, s);
    s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM0, XMM1, s);
    s<<JBE<<" "<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);  
  }
  if(e1 -> getType() == Int && e2 -> getType() == Float){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;
    emit_mrmov(RBP ,e1add, RAX,s);
    emit_int_to_float(RAX, XMM0, s); 
    s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM1, XMM0, s);
    s<<JBE<<" "<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);
  } 
 
}

void Equ_class::code(ostream &s) {
  e1 -> code(s);
  int e1add = tempadd;
  e2 -> code (s);
  int e2add = tempadd;

  emit_sub("$8", RSP, s);
  offset -= 8;
  tempadd = offset;
  
  if(e1 -> getType() == Int && e2 -> getType() == Int){
    emit_mrmov(RBP,e1add,RAX,s);
    emit_mrmov(RBP,e2add,RDX,s);
    emit_cmp(RDX,RAX,s);
    int pos1 = pos;
    s<<JE<<" "<<POSITION<<pos1<<endl;
    pos++;
    int pos2 = pos;
    pos++;
    emit_mov("$0", RAX, s);
    s<<JMP<<" "<<POSITION<<pos2<< endl;
    s<<POSITION<<pos1<<":"<<endl;
    emit_mov("$1", RAX, s);
    s<<POSITION<<pos2<<":"<<endl;
    emit_rmmov(RAX,offset,RBP,s);
  } 
  if(e1 -> getType() == Float && e2 -> getType() == Float){
    s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM0 << endl;
    s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM0, XMM1, s);
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;
    s<<JE<<" "<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<" "<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<":"<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<":"<<endl;
    emit_rmmov(RAX,offset,RBP,s);    
  }
  if(e1 -> getType() == Float && e2 -> getType() == Int){
    s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM0 << endl;
    emit_mrmov(RBP ,e2add, RAX,s);
    emit_int_to_float(RAX, XMM1, s);
    emit_ucompisd(XMM0, XMM1, s);
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;
    s<<JE<<" "<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<" "<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<":"<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<":"<<endl;
    emit_rmmov(RAX,offset,RBP,s);    
  }
  if(e1 -> getType() == Float && e2 -> getType() == Int){
    s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_mrmov(RBP ,e1add, RAX,s);
    emit_int_to_float(RAX, XMM0, s);
    emit_ucompisd(XMM0, XMM1, s);
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;
    s<<JE<<" "<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<" "<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<":"<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<":"<<endl;
    emit_rmmov(RAX,offset,RBP,s);    
  }
}

void Neq_class::code(ostream &s) {
  e1 -> code(s);
  int e1add = tempadd;
  e2 -> code (s);
  int e2add = tempadd;

  emit_sub("$8", RSP, s);
  offset -= 8;
  tempadd = offset;
  if(e1->getType()==Int && e2 ->getType()==Int){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;
    emit_mrmov(RBP ,e1add, RAX,s);
    emit_mrmov(RBP ,e2add, RDX,s);
    emit_cmp(RDX,RAX,s);
    s<<JNE<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);
  }
  if(e1 -> getType() == Float && e2 -> getType() == Float){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;
    s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM1 << endl;
    s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM0 << endl;
    emit_ucompisd(XMM0, XMM1, s);
    s<<JNE<<" "<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);   
  }
  if(e1 -> getType() == Float && e2 -> getType() == Int){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;   
    emit_mrmov(RBP ,e2add, RAX,s);
    emit_int_to_float(RAX, XMM0, s);
    s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM0, XMM1, s);
    s<<JNE<<" "<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);  
  }
  if(e1 -> getType() == Int && e2 -> getType() == Float){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;
    emit_mrmov(RBP ,e1add, RAX,s);
    emit_int_to_float(RAX, XMM0, s); 
    s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM1, XMM0, s);
    s<<JNE<<" "<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);
  } 
 
}

void Ge_class::code(ostream &s) {
  e1 -> code(s);
  int e1add = tempadd;
  e2 -> code (s);
  int e2add = tempadd;

  emit_sub("$8", RSP, s);
  offset -= 8;
  tempadd = offset;
  if(e1->getType()==Int && e2 ->getType()==Int){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;
    emit_mrmov(RBP ,e1add, RAX,s);
    emit_mrmov(RBP ,e2add, RDX,s);
    emit_cmp(RDX,RAX,s);
    s<<JLE<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JGE<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);
  }
  if(e1 -> getType() == Float && e2 -> getType() == Float){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;
    s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM1 << endl;
    s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM0 << endl;
    emit_ucompisd(XMM0, XMM1, s);
    s<<JAE<<" "<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);   
  }
  if(e1 -> getType() == Float && e2 -> getType() == Int){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;   
    emit_mrmov(RBP ,e2add, RAX,s);
    emit_int_to_float(RAX, XMM0, s);
    s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM0, XMM1, s);
    s<<JAE<<" "<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);  
  }
  if(e1 -> getType() == Int && e2 -> getType() == Float){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;
    emit_mrmov(RBP ,e1add, RAX,s);
    emit_int_to_float(RAX, XMM0, s); 
    s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM1, XMM0, s);
    s<<JAE<<" "<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);
  } 
 
}

void Gt_class::code(ostream &s) {
  e1 -> code(s);
  int e1add = tempadd;
  e2 -> code (s);
  int e2add = tempadd;

  emit_sub("$8", RSP, s);
  offset -= 8;
  tempadd = offset;
  if(e1->getType()==Int && e2 ->getType()==Int){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;
    emit_mrmov(RBP ,e1add, RAX,s);
    emit_mrmov(RBP ,e2add, RDX,s);
    emit_cmp(RDX,RAX,s);
    s<<JLE<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JG<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);
  }
  if(e1 -> getType() == Float && e2 -> getType() == Float){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;
    s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM1 << endl;
    s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM0 << endl;
    emit_ucompisd(XMM0, XMM1, s);
    s<<JA<<" "<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);   
  }
  if(e1 -> getType() == Float && e2 -> getType() == Int){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;   
    emit_mrmov(RBP ,e2add, RAX,s);
    emit_int_to_float(RAX, XMM0, s);
    s << MOVSD << e1add << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM0, XMM1, s);
    s<<JA<<" "<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);  
  }
  if(e1 -> getType() == Int && e2 -> getType() == Float){
    int pos1 = pos;
    pos++;
    int pos2 = pos;
    pos++;
    emit_mrmov(RBP ,e1add, RAX,s);
    emit_int_to_float(RAX, XMM0, s); 
    s << MOVSD << e2add << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM1, XMM0, s);
    s<<JA<<" "<<POSITION<<pos1<<endl;
    emit_mov("$0",RAX,s);
    s<<JMP<<POSITION<<pos2<<endl;
    s<<POSITION<<pos1<<':'<<endl;
    emit_mov("$1",RAX,s);
    s<<POSITION<<pos2<<':'<<endl;
    emit_rmmov(RAX, offset, RBP, s);
  }
 
}

void And_class::code(ostream &s) {
  e1 -> code(s);
  int e1add = tempadd;
  e2 -> code (s);
  int e2add = tempadd;

  emit_sub("$8", RSP, s);
  offset -= 8;
  tempadd = offset;
  emit_mrmov(RBP ,e1add, RAX,s);
  emit_mrmov(RBP ,e2add, RDX,s);
  emit_add(RAX,RDX,s);
  emit_rmmov(RDX, offset, RBP, s);

}

void Or_class::code(ostream &s) {
  e1 -> code(s);
  int e1add = tempadd;
  e2 -> code (s);
  int e2add = tempadd;

  emit_sub("$8", RSP, s);
  offset -= 8;
  tempadd = offset;
  emit_mrmov(RBP ,e1add, RAX,s);
  emit_mrmov(RBP ,e2add, RDX,s);
  emit_or(RAX,RDX,s);
  emit_rmmov(RDX, offset, RBP, s);
}

void Xor_class::code(ostream &s) {
  e1 -> code(s);
  int e1add = tempadd;
  e2 -> code (s);
  int e2add = tempadd;

  emit_sub("$8", RSP, s);
  offset -= 8;
  tempadd = offset;
  emit_mrmov(RBP ,e1add, RAX,s);
  emit_mrmov(RBP ,e2add, RDX,s);
  emit_xor(RAX,RDX,s);
  emit_rmmov(RDX, offset, RBP, s);
}

void Not_class::code(ostream &s) {
  e1 -> code(s);
  int e1add = tempadd;

  emit_sub("$8", RSP, s);
  offset -= 8;
  tempadd = offset;
  emit_mrmov(RBP ,e1add, RAX,s);
  emit_mov("$0x0000000000000001", RDX,s);
  emit_xor(RDX,RAX,s);
  emit_rmmov(RAX, offset, RBP, s);
}

void Bitnot_class::code(ostream &s) {
  e1 -> code(s);
  int e1add = tempadd;

  emit_sub("$8", RSP, s);
  offset -= 8;
  tempadd = offset;
  emit_mrmov(RBP ,e1add, RAX,s);
  emit_not(RAX,s);
  emit_rmmov(RAX, offset, RBP, s);
}

void Bitand_class::code(ostream &s) {
  e1 -> code(s);
  int e1add = tempadd;
  e2 -> code (s);
  int e2add = tempadd;

  emit_sub("$8", RSP, s);
  offset -= 8;
  tempadd = offset;
  emit_mrmov(RBP ,e1add, RAX,s);
  emit_mrmov(RBP ,e2add, RDX,s);
  emit_and(RAX,RDX,s);
  emit_rmmov(RDX, offset, RBP, s);

}

void Bitor_class::code(ostream &s) {
  e1 -> code(s);
  int e1add = tempadd;
  e2 -> code (s);
  int e2add = tempadd;

  emit_sub("$8", RSP, s);
  offset -= 8;
  tempadd = offset;
  emit_mrmov(RBP ,e1add, RAX,s);
  emit_mrmov(RBP ,e2add, RDX,s);
  emit_or(RAX,RDX,s);
  emit_rmmov(RDX, offset, RBP, s);
 
}

void Const_int_class::code(ostream &s) {
  emit_sub("$8",RSP,s);
  offset -= 8;
  tempadd = offset;

  s<<MOV<<"$"<<value<<COMMA<<RAX<<endl;
  
  emit_rmmov(RAX, tempadd, RBP, s);
 
}

void Const_string_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  offset -= 8;
  tempadd = offset;
  s<<MOV;
  stringtable.lookup_string(value->get_string())->code_ref(s);
  s<<COMMA<<RAX<<endl;

  emit_rmmov(RAX, tempadd, RBP, s);
 
}

void Const_float_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  offset -= 8;
  tempadd = offset;

  double d_value = atof(value->get_string());
  unsigned long long hex_value = *(unsigned long long *) &d_value;
  char test[17];
  sprintf(test, "%llx", hex_value);
  
  s<<MOV<<"$0x";
  s<<test;
  s<<COMMA<<RAX<<endl;

  emit_rmmov(RAX, tempadd, RBP, s);
 
}

void Const_bool_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  offset -= 8;
  tempadd = offset;

  s<<MOV<<"$"<<value<<COMMA<<RAX<<endl;

  emit_rmmov(RAX, tempadd, RBP, s); 
}

void Object_class::code(ostream &s) {
  if(variabletable.lookup(var)==NULL){
    if(fun){
      strcpy(gol[para],var->get_string());
      flag[para]=1;
    }
    else{
      strcpy(goo,var->get_string());
      flag1=1;
    }
  }
  else
 {tempadd = *variabletable.lookup(var);}
}

void No_expr_class::code(ostream &s) {

}