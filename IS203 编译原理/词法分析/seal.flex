 /*
  *  The scanner definition for seal.
  */

 /*
  *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
  *  output, so headers and global definitions are placed here to be visible
  * to the code in the file.  Don't remove anything that was here initially
  */
%{

#include <seal-parse.h>
#include <stringtab.h>
#include <utilities.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>

/* The compiler assumes these identifiers. */
#define yylval seal_yylval
#define yylex  seal_yylex

/* Max size of string constants */
#define MAX_STR_CONST 256
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the seal compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE seal_yylval;


/*
 *  Add Your own definitions here
 */


%}

%option noyywrap
 /*
  * Define names for regular expressions here.
  */
%Start          STRING
%Start          STR
%Start          COMMENT

%%
 /*comment*/
"//" {
  char c;
  int done = 0;
  do{
    c=yyinput();
    if(c=='\n') {curr_lineno++; done=1;}
  } while(!done);
}

 /*comment*/
  <INITIAL>"/*" {BEGIN COMMENT;}
  <COMMENT><<EOF>> {
	BEGIN 0;
	strcpy(seal_yylval.error_msg, "EOF in comment");
	return ERROR;
 }
 <COMMENT>"*/" { BEGIN INITIAL; }
 <COMMENT>\n { curr_lineno++; }
 <COMMENT>[^*\n]* { } 
 <COMMENT>"*" { }

 /* string*/

 <INITIAL>(\") {
    BEGIN STRING;
    yymore();
}
 <STRING><<EOF>> {
    strcpy(seal_yylval.error_msg, "EOF in string constant");
    BEGIN 0;
    yyrestart(yyin);
    return (ERROR);
}


 <STRING>[^\\\"\n]* { yymore(); }
 <STRING>\n {
   char c;
    /*while((c=yyinput())!='"');*/
   strcpy(seal_yylval.error_msg,"newline in quotation must use a '\\'"); 
   BEGIN 0;
   curr_lineno++;
	 return (ERROR); 
 }
  <STRING>\\0 {
    char c;
    while((c=yyinput())!='"');
    BEGIN 0;
   strcpy(seal_yylval.error_msg,"String contains null character '\\0'"); 
	 return (ERROR); 
 }
   <STRING>\0 {
     char c;
     while((c=yyinput())!='"');
   strcpy(seal_yylval.error_msg,"String contains null character '\\0'"); 
   BEGIN 0;
	 return (ERROR); 
 }

 <STRING>\\[^\n] { yymore(); }




 <STRING>\\\n {
   curr_lineno++;
   yymore();
 }

 <STRING>(\") {
   std::string input(yytext,yyleng);
   input = input.substr(1, input.length() - 2);

   std::string output="";
   std::string::size_type pos;
    while ((pos = input.find_first_of("\\")) != std::string::npos) {
        output += input.substr(0, pos);

        switch (input[pos + 1]) {
        case 'b':
            output += "\b";
            break;
        case 't':
            output += "\t";
            break;
        case 'n':
            output += "\n";
            break;
        case 'f':
            output += "\f";
            break;
        default:
            output += input[pos + 1];
            break;
        }

        input = input.substr(pos + 2, input.length() - 2);
    }

    output += input;

    if (output.length() > MAX_STR_CONST) {
        strcpy(yylval.error_msg  ,"String is too long");
        BEGIN 0;
        return ERROR;    
    }

   seal_yylval.symbol = stringtable.add_string((char*)output.c_str());
   BEGIN 0;
   return (CONST_STRING);
 }

  <INITIAL>(\`) {
    BEGIN STR;
    yymore();
  }
   <STR><<EOF>> {
    strcpy(seal_yylval.error_msg, "EOF in string constant");
    BEGIN 0;
    yyrestart(yyin);
    return (ERROR);
}
  <STR>[^`\n]+ {
    yymore();
  }
  <STR>\n {
    curr_lineno++;
    yymore();
  }
  <STR>(\`) {
   std::string input(yytext,yyleng);
   input = input.substr(1, input.length() - 2);
   std::string output="";
   std::string::size_type pos;
    while ((pos = input.find_first_of("\\")) != std::string::npos){
      output += input.substr(0, pos);
      output+="\\";
      input = input.substr(pos + 1, input.length() - 1);
    }
    /* while ((pos = input.find_first_of("\n")) != std::string::npos){
      output += input.substr(0, pos-1);
      output+="\\n";
      input = input.substr(pos + 1, input.length() - 1);
    }*/
   output += input;
   if (output.length() > MAX_STR_CONST) {
      strcpy(yylval.error_msg  ,"String is too long");
      BEGIN 0;
      return ERROR;    
    }
   seal_yylval.symbol = stringtable.add_string((char*)output.c_str());
   BEGIN 0;
   return (CONST_STRING);

  }

\n {curr_lineno++;}
= {return int('=');}
"+" {return int('+');}
"-" {return int('-');}
"*" {return int('*');}
"/" {return int('/');}
"%" {return int('%');}
"&" {return int('&');}
"|" {return int('|');}
"^" {return int('^');}
"~" {return int('~');}
"!" {return int('!');}
">" {return int('>');}
"<" {return int('<');}

"," {return int(',');}
"(" {return int('(');}
")" {return int(')');}
"{" {return int('{');}
"}" {return int('}');}
"&&" {return AND;}
"||" {return OR;}
"==" {return EQUAL;}
"!=" {return NE;}
">=" {return GE;}
"<=" {return LE;}
var {return VAR;}
Int {seal_yylval.symbol = idtable.add_string(yytext); 	return (TYPEID);}
Float {seal_yylval.symbol = idtable.add_string(yytext); 	return (TYPEID);}
Bool {seal_yylval.symbol = idtable.add_string(yytext); 	return (TYPEID);}
String {seal_yylval.symbol = idtable.add_string(yytext); 	return (TYPEID);}
Void {seal_yylval.symbol = idtable.add_string(yytext); 	return (TYPEID);}
if {return IF;}
else {return ELSE;}
while {return WHILE;}
for {return FOR;}
break {return BREAK;}
continue {return CONTINUE;}
func {return FUNC;}
return {return RETURN;}
; {return int(';');}
[ ]+ {}
["\t"]+ {}

"true"  {seal_yylval.boolean = true; 	return (CONST_BOOL);}
"false"  {seal_yylval.boolean = false; 	return (CONST_BOOL);}

[0-9]+ {seal_yylval.symbol = inttable.add_string(yytext); 	return (CONST_INT);}
0[x|X][0-9|a-f|A-F]+ {
  int i,m,temp=0,n;
  m=strlen(yytext);
  for(i=2;i<m;i++)
 {
  if(yytext[i]>='A'&&yytext[i]<='F')//十六进制还要判断字符是不是在A-F或者a-f之间
  {
    n=yytext[i]-'A'+10;
  }
  else if(yytext[i]>='a'&&yytext[i]<='f')
  {
    n=yytext[i]-'a'+10;
  }
  else
  {
    n=yytext[i]-'0';
  } 

  temp=temp*16+n;
 }
    char res[1000];
    int j = 0;
    do
        res[j++] = '0' + temp % 10;
    while ((temp/= 10));
    res[j] = '\0';
    for (int i = 0; i < (j + 1) / 2; ++i) {
        char t = res[i]; res[i] = res[j - 1 - i]; res[j - 1 - i] = t;
    }
seal_yylval.symbol = inttable.add_string(res); 	
return (CONST_INT);
}

[0-9]+.[0-9]+ {seal_yylval.symbol = floattable.add_string(yytext); 	return (CONST_FLOAT);}

[a-z][a-z|A-Z|0-9|_]* {seal_yylval.symbol = idtable.add_string(yytext); 	return (OBJECTID);}



.	{
	strcpy(seal_yylval.error_msg, yytext); 
	return (ERROR); 
}

%%
