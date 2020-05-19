#ifndef SITE_H
#define SITE_H

typedef struct _site {
	char* fqdn;
	char* dossier;
	char* e400;
	char* e404;
	char* e411;
	char* e418;
	char* e501;
	char* e505;
	struct _site* next;
} Site;

Site* multisitesConf;
void loadMultisitesConf();
void unloadMultiSitesConf();

#endif