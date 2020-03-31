#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

#include "api.h" 
#include "tree.h"
#include "highLevel.h"

int main(int argc,char *argv[])
{
	////////////////////////MAIN CONSIGNE
	int res,fi;
	char *p=NULL,*addr;

	
        struct stat st;

	if (argc < 2 ) { 
		printf("Usage: httpparser <file> <search>\nAttention <search> is case sensitive\n"); 
		exit(1); 
	}

	/* ouverture du fichier contenant la requête */ 
	if ((fi=open(argv[1],O_RDWR)) == -1) {
                perror("open");
                return FALSE;
        }
        if (fstat(fi, &st) == -1)           /* To obtain file size */
                return FALSE;
        if ((addr=mmap(NULL,st.st_size,PROT_WRITE,MAP_PRIVATE, fi, 0)) == NULL )
                return FALSE;

    if (argc == 2 ) {
    	Node *testNode = malloc(sizeof(Node));
		testNode->childList = malloc(sizeof(Node)*MAX_CHILD_COUNT);
		testNode->childCount = 0;
		testNode->contentSize = 0;
		
		if(HTTPmessage(addr,testNode))
			printTree(testNode,0);
		else printf("/!\\ Erreur de syntaxe\n");
		removeNode(testNode);
		return 0;
    }

    	// This is a special HACK since identificateur in C can't have character '-'
	if (argc == 3 ) { 
		p=argv[2]; 	
		printf("searching for %s\n",p); 
		while (*p) { 
			if (*p=='-') { *p='_'; }
			p++; 
		}
		p=argv[2]; 	
	
	// call parser and get results. 
	if ((res=parseur(addr,st.st_size))) {
		_Token *r,*tok; 
		void *root=NULL;
		root=getRootTree(); 
		r=searchTree(root,p); 
		tok=r; 
		while (tok) {
			int l; 
			char *s; 
			s=getElementValue(tok->node,&l); 
			printf("FOUND [%.*s]\n",l,s);
			tok=tok->next; 
		}
		purgeElement(&r);
		purgeTree(root);
	}
	close(fi);
	return(res); 
	}
}
