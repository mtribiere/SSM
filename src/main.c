#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <unistd.h>
//#include <sys/mman.h>
//#include <fcntl.h>
//#include <errno.h>

#include "api.h" 
#include "header.h"
#include "baseLevel.h"
#include "highLevel.h"
#include "parser.h"

#define false 0 


int main(int argc,char *argv[])
{
	////////////////////////MAIN CONSIGNE
	// int res,i,fi;
	// char *p=NULL,*addr;

	
    //     struct stat st;

	// if (argc < 3 ) { printf("Usage: httpparser <file> <search>\nAttention <search> is case sensitive\n"); exit(1); }
	// /* ouverture du fichier contenant la requête */ 
	// if ((fi=open(argv[1],O_RDWR)) == -1) {
    //             perror("open");
    //             return false;
    //     }
    //     if (fstat(fi, &st) == -1)           /* To obtain file size */
    //             return false;
    //     if ((addr=mmap(NULL,st.st_size,PROT_WRITE,MAP_PRIVATE, fi, 0)) == NULL )
    //             return false;

	// // This is a special HACK since identificateur in C can't have character '-'

	// if (argc == 3 ) { 
	// 	p=argv[2]; 	
	// 	printf("searching for %s\n",p); 
	// 	while (*p) { 
	// 		if (*p=='-') { *p='_'; }
	// 		p++; 
	// 	}
	// 	p=argv[2]; 	
	// }
	// // call parser and get results. 
	// if (res=parseur(addr,st.st_size)) {
	// 	_Token *r,*tok; 
	// 	void *root=NULL;
	// 	root=getRootTree(); 
	// 	r=searchTree(root,p); 
	// 	tok=r; 
	// 	while (tok) {
	// 		int l; 
	// 		char *s; 
	// 		s=getElementValue(tok->node,&l); 
	// 		printf("FOUND [%.*s]\n",l,s);
	// 		tok=tok->next; 
	// 	}
	// 	purgeElement(&r);
	// 	purgeTree(root);
	// }
	// close(fi);
	// return(res); 

	////////////////////TEST FONCTIONS
	char s[] = "::0000:0000:0000:0000:0000";
	Node testNode;
	testNode.childList = malloc(sizeof(Node)*MAX_CHILD_COUNT);
	testNode.childCount = 0;
	testNode.contentSize = 0;

	if(IPv6address(s,&testNode))
		printTree(&testNode,0);
	else printf("FAUX\n");

	removeNode(&testNode);
	
	return 0;

	/////////////////TEST API
	/*
	char s[] = "GET / HTTP/1.1\r\n";
	parseur(s,strlen(s));

	void *treeRoot = getRootTree();

	printf("Resulat : %s\n",getElementValue(treeRoot,NULL));
	//Attention la fct getElementValue() a été commentée
	*/
	

}
