/*
	Contiene código de lexer y parser en uno
	para compilar $gcc -o parser parser.c
*/


#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
	
/*	Definición de componentes léxicos
	Código 2xx tokens válidos
	Código 4xx EOF
	Código 5xx tokens invalidos */
enum lex_code{
		L_CORCHETE = 200, 
		R_CORCHETE = 201,
		L_LLAVE = 202,
		R_LLAVE = 203,
		COMA = 204,
		DOS_PUNTOS = 205,
		LITERAL_CADENA = 206,
		LITERAL_NUM = 207,
		PR_TRUE = 208,
		PR_FALSE = 209,
		PR_NULL = 210,
		eof = 400,
		ERROR = 500
};

/* Retorna componente lexico */
char * to_lex_component(enum lex_code lexcode){

	char *lex_component;
	switch(lexcode){
		case 200:
			lex_component = "L_CORCHETE";
			break;
		case 201:
			lex_component = "R_CORCHETE";
			break;
		case 202:
			lex_component = "L_LLAVE";
			break;
		case 203:
			lex_component = "R_LLAVE";
			break;
		case 204:
			lex_component = "COMA";
			break;
		case 205:
			lex_component = "DOS_PUNTOS";
			break;
		case 206:
			lex_component = "LITERAL_CADENA";
			break;
		case 207:
			lex_component = "LITERAL_NUM";
			break;
		case 208:
			lex_component = "PR_TRUE";
			break;
		case 209:
			lex_component = "PR_FALSE";
			break;
		case 210:
			lex_component = "PR_NULL";
			break;
		case 400:
			lex_component = "EOF";
			break;
		default:
			lex_component = "ERROR";
			break;
	}
	return lex_component;
}


/*	Define un tipo nodo lexema.
	Componenetes:
	1. lexcode : código del lexema
	2. lex_content : contenido real del lexema
	3. prev_blanks : contiene 
	*/
typedef struct lex_node{
	int lexcod;
	char *lex_content;
	char prev_blanks[100];
	struct lex_node *next;

}lexema;


/*	Inicializa un nodo lexema */
void init_lexema(lexema * l){
	l->lexcod = 0;
	l->lex_content = NULL;
	l->prev_blanks[0] = '\0';
	l->next = NULL;
}

/*A better name may print_lexema */
void imprimir(lexema *lista){

	while(lista){

		printf("%s", lista->prev_blanks);
		printf("%s ", to_lex_component(lista->lexcod));
		lista = lista->next;
	}
	printf("\n");
}

void concatenar( char* dest, char c )
{
	int indice = 0;
    while( dest[indice] != '\0') indice++;

    dest[indice] = c;
    dest[++indice] = '\0';
}

int longitud(char *str){
	int indice = 0;
	while( str[indice] != '\0') indice++;
	return indice;
}

void copiar_str(){

}

lexema * lexer(FILE *fuente){

	int error = 0; //Cantidad de errores encontrados
	int linea = 1; //línea que se está procesando
	char c = -1; //caracter analizado

	lexema *lex_list = NULL; //Lista de lexemas
	lexema *n_actual = malloc(sizeof(lexema));
	init_lexema(n_actual);
	lex_list = n_actual;
	
	int evaluado = 0;  //Existe un char pendiente de evaluación

	while((c=fgetc(fuente))!=EOF){

		if( c == ' ' || c == '\t' || c == '\n'){

			/*Concatenar los blancos en la variable blancos*/
			concatenar(n_actual->prev_blanks, c);

			if( c == '\n'){
				linea++;
			}
		} 
		/**
		 	Caso Literal cadena; busca comilla de cierre hasta el final de la
		  	línea. Error si no existe
		  */
		else if( c == '"' ){ //caso string
			
			c=fgetc(fuente);

			while(c != '"' && c != EOF && c != '\n'){
				//str = str = c; Concatenar
				c=fgetc(fuente);
			}

			switch(c){
				case '"':
					//str = str + c;
					n_actual->lexcod = LITERAL_CADENA;
					break;
				case EOF:
					error++;
					printf("Error en linea %d: Se espera \", se ha encontrado EOF\n", linea);
					exit(1);
					break;
				case '\n':
					error++;
					printf("Error en linea %d: Se espera \", se ha encontrado \\n\n", linea);
					linea ++;
					break;
			}

		}
		/* Caso false */
		else if( c == 'f' ){

			//char *f_string = "(false|FALSE)";
			char *f_string = "false";
			regex_t regex;
			int resultado;
			char str[50];
			str[0] = '\0';

			while(longitud(str) < longitud(f_string) && c != '\n' && c != EOF){

				concatenar(str, c);
				resultado = regcomp(&regex, str, REG_EXTENDED);                                     
				if(resultado){ exit(1);} //Error de compilación
				resultado = regexec(&regex, f_string, 0, NULL, 0);

				if(longitud(str) < longitud(f_string)) c = fgetc(fuente);
			}


			if( resultado == 0 && longitud(str) == longitud(f_string)){
				n_actual->lexcod = PR_FALSE;
			}
			else{
				printf(str);//Test purpose
				error++;

				while( c != '\n' && c != EOF){
					c=fgetc(fuente);
				}

				if(c == '\n') concatenar(n_actual->prev_blanks, c);
				printf("Error en linea %d: lexema desconocido\n", linea);
				linea++;			
			}
			regfree(&regex);
		}
		/* Caso FALSE */
		else if( c == 'F' ){

			//char *f_string = "(false|FALSE)";
			char *f_string = "FALSE";
			regex_t regex;
			int resultado;
			char str[50];
			str[0] = '\0';

			while(longitud(str) < longitud(f_string) && c != '\n' && c != EOF){

				concatenar(str, c);
				resultado = regcomp(&regex, str, REG_EXTENDED);                                     
				if(resultado){ exit(1);} //Error de compilación
				resultado = regexec(&regex, f_string, 0, NULL, 0);

				if(longitud(str) < longitud(f_string)) c = fgetc(fuente);
			}


			if( resultado == 0 && longitud(str) == longitud(f_string)){
				n_actual->lexcod = PR_FALSE;
			}
			else{
				printf(str);//Test purpose
				error++;

				while( c != '\n' && c != EOF){
					c=fgetc(fuente);
				}

				if(c == '\n') concatenar(n_actual->prev_blanks, c);
				printf("Error en linea %d: lexema desconocido\n", linea);
				linea++;			
			}
			regfree(&regex);
		}
		/* Caso true */
		else if( c == 't' ){

			//char *f_string = "(false|FALSE)";
			char *f_string = "true";
			regex_t regex;
			int resultado;
			char str[50];
			str[0] = '\0';

			while(longitud(str) < longitud(f_string) && c != '\n' && c != EOF){

				concatenar(str, c);
				resultado = regcomp(&regex, str, REG_EXTENDED);                                     
				if(resultado){ exit(1);} //Error de compilación
				resultado = regexec(&regex, f_string, 0, NULL, 0);

				if(longitud(str) < longitud(f_string)) c = fgetc(fuente);
			}


			if( resultado == 0 && longitud(str) == longitud(f_string)){
				n_actual->lexcod = PR_FALSE;
			}
			else{
				printf(str);//Test purpose
				error++;

				while( c != '\n' && c != EOF){
					c=fgetc(fuente);
				}

				if(c == '\n') concatenar(n_actual->prev_blanks, c);
				printf("Error en linea %d: lexema desconocido\n", linea);
				linea++;			
			}
			regfree(&regex);
		}
		/* Caso TRUE */
		else if( c == 'T' ){

			//char *f_string = "(false|FALSE)";
			char *f_string = "TRUE";
			regex_t regex;
			int resultado;
			char str[50];
			str[0] = '\0';

			while(longitud(str) < longitud(f_string) && c != '\n' && c != EOF){

				concatenar(str, c);
				resultado = regcomp(&regex, str, REG_EXTENDED);                                     
				if(resultado){ exit(1);} //Error de compilación
				resultado = regexec(&regex, f_string, 0, NULL, 0);

				if(longitud(str) < longitud(f_string)) c = fgetc(fuente);
			}


			if( resultado == 0 && longitud(str) == longitud(f_string)){
				n_actual->lexcod = PR_FALSE;
			}
			else{
				error++;

				while( c != '\n' && c != EOF){
					c=fgetc(fuente);
				}

				if(c == '\n') concatenar(n_actual->prev_blanks, c);
				printf("Error en linea %d: lexema desconocido\n", linea);
				linea++;			
			}
			regfree(&regex);
		}
		//TODO - CASO LITERAL NUMERICO
		else if ( c >= 48 && c <= 57){

			while(c >= 48 && c <= 57){
				c=fgetc(fuente);
			}
			n_actual->lexcod = LITERAL_NUM;


			/* Si existe una coma (,) pegada a un LITERAL_NUM*/
			if( c == ','){
				n_actual->next = malloc(sizeof(lexema));
				n_actual = n_actual->next;
				init_lexema(n_actual);
				n_actual->lexcod = COMA;
			}
			else if( c != ' ' && c != '\t' && c != '\n'){
				printf("Error en linea %d: lexema desconocido\n", linea);
				linea++;			
			}

		}
		else{

			switch(c){
				case '[':
					n_actual->lexcod = L_CORCHETE;
					break;
				case ']':
					n_actual->lexcod = R_CORCHETE;
					break;
				case '{':
					n_actual->lexcod = L_LLAVE;
					break;
				case '}':
					n_actual->lexcod = R_LLAVE;
					break;
				case ',':
					n_actual->lexcod = COMA;
					break;
				case ':':
					n_actual->lexcod = DOS_PUNTOS;
					break;
				default:
					while( c != '\n' && c != EOF){
						c = fgetc(fuente);
					}

					/* Encuentra el final de línea o el final del archivo
						antes de cerrar e*/
					if( c == '\n' || c == EOF){
						error++;
						printf("Error en linea %d: lexema desconocido\n", linea);
						linea++;
					}
					else{ //EOF
						error++;
						printf("Error en linea %d: lexema desconocido\n", linea);
					}
			}
		}

		/**
		 * Crea un nuevo nodo de lexema si el último ha sido usado.
		 * Actualiza los punteros correspondientes.
		 * */
		if( n_actual->lexcod != 0){
			n_actual->next = malloc(sizeof(lexema));
			n_actual = n_actual->next;
			init_lexema(n_actual);

			if( c == ' ' || c == '\t' || c == '\n' )
				concatenar(n_actual->prev_blanks, c);

		}
	}

	/**
	 * Se llega al final de archivo. Se agrega el componente
	 * léxico al final de las lista.
	 * */
	if( c == EOF ){
		n_actual->lexcod = eof;
	}

	return lex_list;

	//imprimir(lex_list);
}

//-----------------------------------------------------FIN LEXER-----------------------------------------------------

//---------------------------------------------------INICIO PARSER---------------------------------------------------


void element();
void json();
void object();
void array();
void element_list();
void attribute_list();
void attribute();
void attribute_value();
void attribute_name();
void sincronizar();


lexema* token;
lexema* primer_token;
int token_inicial;
int errores = 0;

lexema* get_token(){

	return token->next;
}

void error(int lex){

	errores++;
	printf("%s%s%s%s\n", "Match error: encontrado ", to_lex_component(token->lexcod), \
		" esperado ", to_lex_component(lex));
	sincronizar(lex);
	//exit(1);
}

void sincronizar(int lex){
	token = get_token();

	while(token->lexcod != lex && token->lexcod != EOF){
		token = get_token();
	}

	if(token->lexcod == lex)
		printf("Sincronizando : %s\n", to_lex_component(lex));
	else{
		printf("Finlizado con %d errores\n", errores);
		exit(1);
	}
}

void match(int lex){
	if(token->lexcod != lex){ error(lex); }
	printf("Match : %d , %s\n", token->lexcod, to_lex_component(token->lexcod));
}

void parser(lexema* token_list){

	primer_token = token_list;
	token = token_list; //token apunta al inicio
	token_inicial = 1;
	json();
}

void json(){

	element();
	token = get_token();
	match(eof);

	if(errores == 0)
		printf("%s\n", "Aceptado");
	else{
		printf("Finlizado con %d errores\n", errores);
		exit(1);
	}	

}

void element(){
	
	//Element no debe invocar a get_token()
	//printf("Invoca a element con token : %d , %s\n", token->lexcod, to_lex_component(token->lexcod));
	switch(token->lexcod){
		//element=>object
		//case '{':
		case L_LLAVE:
			object();
			break;
		//element=>array
		//case '[':
		case L_CORCHETE:
			array();
			break;
		default:
			error(500);
	}

}

void object(){

	//printf("Invoca a object con token : %d , %s\n", token->lexcod, to_lex_component(token->lexcod));
	//Verifica si el token actual es un {
	match(L_LLAVE);
		token = get_token();
		attribute_list();
	match(R_LLAVE);
	
	/*
	switch(token->lexcod){
		case R_LLAVE:
			match(R_LLAVE);
			break;
		default:
			attribute_list();
	}
	match(R_LLAVE);
	*/
}

void array(){

	//printf("Invoca a array con token : %d , %s\n", token->lexcod, to_lex_component(token->lexcod));
	match(L_CORCHETE);
	token = get_token();
	
	switch(token->lexcod){
		case R_CORCHETE: //caso array=>[]
			match(R_CORCHETE);
			//token = get_token();
			break;
		default: { //caso array=>[{element-list}]
			element_list();
			match(R_CORCHETE);
		}
	}
}

void element_list(){

	//printf("Invoca a element_list con token : %d , %s\n", token->lexcod, to_lex_component(token->lexcod));
	element();

	token = get_token();
	while(token->lexcod == COMA){
		match(COMA);
		token = get_token();
		element();
		token = get_token();
	}
}

void attribute_list(){

	//printf("Invoca a attribute_list con token : %d , %s\n", token->lexcod, to_lex_component(token->lexcod));
	attribute();

	token = get_token();
	while(token->lexcod == COMA){
		match(COMA);
		token = get_token();
		attribute();
		token = get_token();
	}
}

void attribute(){

	//attribute => attribute-name : attribute-value
	//printf("Invoca a attribute con token : %d , %s\n", token->lexcod, to_lex_component(token->lexcod));
	attribute_name();
	token = get_token();
	match(DOS_PUNTOS);
	token = get_token();
	attribute_value();
}

void attribute_name(){

	//printf("Invoca a attribute_name con token : %d , %s\n", token->lexcod, to_lex_component(token->lexcod));
	match(LITERAL_CADENA);
}

void attribute_value(){

	//printf("Invoca a attribute_value con token : %d , %s\n", token->lexcod, to_lex_component(token->lexcod));

	switch(token->lexcod){
		case LITERAL_CADENA:
			match(LITERAL_CADENA);
			break;
		case LITERAL_NUM:
			match(LITERAL_NUM);
			break;
		case PR_TRUE:
			match(PR_TRUE);
			break;
		case PR_FALSE:
			match(PR_FALSE);
			break;
		case PR_NULL:
			match(PR_NULL);
			break;
		default:
			element();
	}
}


int main(int argc,char* args[])
{
	FILE *fuente;

	if(argc > 1)
	{
		if (!(fuente=fopen(args[1],"rt")))
		{
			printf("Archivo fuente no encontrado.\n");
			exit(1);
		}

		lexema * lex_list = lexer(fuente);
		
		printf("%s\n", "-------------");

		//primer_token = lex_list;
		//token = lex_list;
		imprimir(lex_list);
		printf("%s\n", "-------------");

		parser(lex_list);

		//imprimir(lex_list);
		fclose(fuente);

	}else{
		printf("Debe pasar como parametro el path al archivo fuente.\n");
		exit(1);
	}

	return 0;
}



/*
BNF
---
json=>element eof
element=>object | array
array=>[element-list] | []
element-list=>element-list , element | element
object=>{attributes-list} | {}
attributes-list=>attribute-list , attribute | attribute
attribute=>attribute-name : attribute-value
attribute-name=>string
attribute-value=>element | string | number | true | false | null

EBNF	
----
json=>element eof
element=>object | array
array=>[{element-list}] | []
element-list=>element {,element}
object=>"{"{attributes-list}"}"
attributes-list=>attribute{,attribute}
attribute=>attribute-name : attribute-value
attribute-name=>string
attribute-value=>element | string | number | true | false | null
*/
