#include "highLevel.h"
#include "baseLevel.h"
#include "parser.h"
#include "tree.h"

int HTTPname(const char *s, Node* node){
    if(regexTest(s,"HTTP",4)){
        node->content = s;
        node->contentSize = 4;
        return TRUE;
    } else {
        removeNode(node);
        return FALSE;
    }}

int HTTPVersion(const char *s, Node* node){
    createChild(node, 5, (char[][NAMESIZE]){"HTTP-name","/","DIGIT",".","DIGIT"});
    if(strlen(s) > 7 && (HTTPname(s, node->childList[0]) && slash(s+4, node->childList[1]) && DIGIT(s+5, node->childList[2]) && dot(s+6 , node->childList[3]) && DIGIT(s+7, node->childList[4]))) {
        node->content = s;
        node->contentSize = 8;
        return TRUE;
    }
    else {removeNode(node);return FALSE;}
}
