




#define LEXICAL_ERROR       		1
#define SYNTAX_ERROR 				2 
#define SEM_ERR_UNDEF   			3 
#define SEM_ERR_NEW_VAR				4 
#define SEM_ERR_TYPE_COMPAT	    	5 
#define SEM_ERR_PAR_NUM	    	    6
#define SEM_ERR_OTHER		    	7 
#define SEM_ERR_DIV_ZERO	        9
#define INTERN_ERROR        		99 
	   
/***************************************************************************
• 1 - chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému).
• 2 - chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu, neo- čekávané odřádkování).
• 3 - sémantická chyba v programu – nedefinovaná funkce/proměnná, pokus o redefi- nici funkce/proměnné, atp.
• 4 - sémantická chyba při odvozování datového typu nově definované proměnné.
• 5 - sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech.
• 6 - sémantickáchybavprogramu–špatnýpočet/typparametrůčinávratovýchhodnot u volání funkce či návratu z funkce.
• 7 - ostatní sémantické chyby.
• 9 - sémantická chyba dělení nulovou konstantou.
• 99 - interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alo- kace paměti, atd.).

*/