/**

 Copyright 2007
 Georgia Tech Research Corporation
 Atlanta, GA  30332-0415
 All Rights Reserved

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are
 met:

 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided
 * with the distribution.

 * Neither the name of the Georgia Tech Research Coporation nor the
 * names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior
 * written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 **/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <errno.h>
#include "include/config_parser.h"
#include "include/dytan.h"
#include "include/struct_dat.h"

void parseRulesFile() {
	bool flag_imgName = true;
	bool flag_APIname = false;
	bool flag_overwrite = false;
	bool flag_sourceArg = false;
	bool flag_destArg = false;
	int count;
	string APIname;
	string imgName;
	char buf[256];
	FILE *fp = NULL;
	fp = fopen("conf_rules.dat", "rt");
	if (fp) {
		memset(buf, 0, sizeof(buf));
		count = fread(buf, 1, sizeof(buf), fp);
		if (!count)
			printf("Error: Empty conf_rules.dat file\n");
		fclose(fp);
		char *token = strtok(buf, ",\n");
		while (token) {
			if (flag_imgName) {
				imgName = token;
				printf("imgName: %s\n", imgName.c_str());
				rulesImageMap[imgName] = new rulesMap;
				flag_imgName = false;
				flag_APIname = true;
			} else if (flag_APIname) {
				APIname = token;
				printf("APIname: %s\n", APIname.c_str());
				(*(rulesImageMap[imgName]))[APIname] = new rules;
				flag_APIname = false;
				flag_overwrite = true;
			} else if (flag_overwrite) {
				int read = atoi(token);
				if (read) {
					printf("Overwrite enabled\n");
					((*(rulesImageMap[imgName]))[APIname])->overwrite = true;
				} else {
					printf("asssigning the union ...\n");
					((*(rulesImageMap[imgName]))[APIname])->overwrite = false;
				}
				flag_overwrite = false;
				flag_sourceArg = true;
			} else if (flag_sourceArg) {
				int read = atoi(token);
				if (read == -1) {
					printf("-1 in sourceArg\n");
					flag_sourceArg = false;
					flag_destArg = true;
				} else {
					((*(rulesImageMap[imgName]))[APIname])->srcArgs.push_back(
							read);
					printf("pushed back source arg: %d\n", read);
				}
			} else if (flag_destArg) {
				int read = atoi(token);
				if (read == -1) {
					printf("-1 in destArg\n");
					flag_destArg = false;
					flag_imgName = true;
				} else {
					((*(rulesImageMap[imgName]))[APIname])->destArgs.push_back(
							read);
					printf("pushed back dest arg: %d\n", read);
				}
			}
			token = strtok(NULL, ",\n");
		}
	} else {
		printf("Error: Cannot open conf_rules.dat\n");
	}
}

datfile datFile;

void parseDatFiles() {
	char buf[256];
	int count;
	int index = 0;
	FILE *fp = NULL;
	fp = fopen("conf_offset.dat", "rt");
	if (fp) {
		memset(buf, 0, sizeof(buf));
		count = fread(buf, 1, sizeof(buf), fp);
		if (count == 0)
			printf("Error: empty conf_offset.dat file!\n");
		fclose(fp);
		char *token = strtok(buf, ",\n");
		count = 0;
		while (token) {
			if (count % 3 == 0) {
				// option name
			}
			if (count % 3 == 1) {
				// start_offset
				datFile.start[index] = atoi(token);
				printf("word[%d]_start_offset = %d\n", index,
						datFile.start[index]);
			}
			if (count % 3 == 2) {
				// end_offset
				datFile.end[index] = atoi(token);
				printf("word[%d]_end_offset = %d\n", index, datFile.end[index]);
				index++;
			}
			token = strtok(NULL, ",\n");
			count++;
		}
		datFile.index = index - 1;
	} else
		printf("Error: cannot open conf_offset.dat file!\n");
}

/**
 * Parses "sources" child of the xml configuration
 */
void parseSources(xmlDocPtr doc, xmlNodePtr cur, config *conf) {
	xmlChar *txt, *subtext;
	xmlNodePtr sub;
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "source"))) {
			source src;
			txt = xmlGetProp(cur, (const xmlChar *) "type");
			if (txt) {
				logfile << "sources type = " << txt << ":\n";
				logfile.flush();
				string type((char *) txt);
				src.type = type;
				xmlFree(txt);
				sub = cur->xmlChildrenNode;
				while (sub != NULL) {
					if (sub->type == XML_ELEMENT_NODE) {
						logfile << "\t\t " << sub->name << ": ";
						subtext = xmlNodeListGetString(doc,
								sub->xmlChildrenNode, 1);
						logfile << subtext << "\n";
						logfile.flush();
						if (!xmlStrcmp(sub->name,
								(const xmlChar *) "granularity")) {
							string granularity((char *) subtext);
							if (granularity == "PerOffset")
								parseDatFiles();
							src.granularity = granularity;
						} else {
							string details((char *) subtext);
							src.details.push_back(details);
						}
						xmlFree(subtext);
					}
					sub = sub->next;
				}
			} else {
				fprintf(stderr, "Sources: Document not structured properly");
			}
			conf->sources.push_back(src);
		} else if ((!xmlStrcmp(cur->name, (const xmlChar *) "taint-marks"))) {
			txt = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			logfile << "max taint marks = " << txt << "\n";
			logfile.flush();
			string num_markings((const char *) txt);
			conf->num_markings = num_markings;
			xmlFree(txt);
		}
		cur = cur->next;
	}
}

/**
 * Parses "Propagation" child of the configuration 
 */
void parsePropagation(xmlDocPtr doc, xmlNodePtr cur, config *conf) {
	xmlChar *key;
	propagation prop;
	prop.dataflow = false;
	prop.controlflow = false;
	prop.tracing = false;
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "dataflow"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			logfile << "\ndataflow :" << key;
			prop.dataflow = (!strcmp((char *) key, "true") ? true : false);
			xmlFree(key);
		} else if ((!xmlStrcmp(cur->name, (const xmlChar *) "controlflow"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			logfile << "\ncontrolflow :" << key;
			prop.controlflow = (!strcmp((char *) key, "true") ? true : false);
			xmlFree(key);
		} else if ((!xmlStrcmp(cur->name, (const xmlChar *) "tracing"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			logfile << "\ntracing :" << key;
			prop.tracing = (!strcmp((char *) key, "true") ? true : false);
			xmlFree(key);
		}
		cur = cur->next;
	}
	conf->prop = prop;
}

/**
 * Parses "Sinks" child of the configuration
 */
void parseSinks(xmlDocPtr doc, xmlNodePtr cur, config *conf) {
	xmlChar *txt;
	xmlNodePtr sub, subsub;
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "sink"))) {
			sub = cur->xmlChildrenNode;
			while (sub != NULL) {
				if ((!xmlStrcmp(sub->name, (const xmlChar *) "id"))) {
					txt = xmlNodeListGetString(doc, sub->xmlChildrenNode, 1);
					logfile << "\nid = " << txt << "\n";
					xmlFree(txt);
				} else if (!xmlStrcmp(sub->name,
						(const xmlChar *) "location")) {
					txt = xmlGetProp(sub, (const xmlChar *) "type");
					if (txt) {
						logfile << "location type = " << txt << ":\n";
						xmlFree(txt);
						subsub = sub->xmlChildrenNode;
						while (subsub != NULL) {
							if (subsub->type == XML_ELEMENT_NODE) {
								logfile << "\t\t " << subsub->name << " ";
								txt = xmlNodeListGetString(doc,
										subsub->xmlChildrenNode, 1);
								logfile << txt << "\n";
								xmlFree(txt);
							}
							subsub = subsub->next;
						}
					}
				}
				sub = sub->next;
			}
		}
		cur = cur->next;
	}
}

#define CONFIGFILE "config.xml"

/**
 * Parse a simple XML document, using libxml2
 *
 */
int parseConfig(int argc, char **argv, config *conf) {

	parseRulesFile();

	xmlDocPtr doc;
	xmlNodePtr cur, sub;

	if (!(doc = xmlParseFile( CONFIGFILE))) {
		fprintf( stderr,
				"Configuration file could not be parsed. It must be present in CURRENT DIRECTORY.\n");
		xmlFreeDoc(doc);
		return (-1);
	}

	if (!(cur = xmlDocGetRootElement(doc))) {
		fprintf( stderr, "Configuration file has no root element.\n");
		xmlFreeDoc(doc);
		return (-1);
	}
	if (xmlStrcmp(cur->name, (const xmlChar *) "dytan-config")) {
		fprintf( stderr,
				"Configuration file of the wrong type, root node != dytan-config\n");
		xmlFreeDoc(doc);
		return (-1);
	}

	sub = cur->children;
	while (sub != NULL) {
		if ((!xmlStrcmp(sub->name, (const xmlChar *) "sources"))) {
			parseSources(doc, sub, conf);
		} else if ((!xmlStrcmp(sub->name, (const xmlChar *) "propagation"))) {
			parsePropagation(doc, sub, conf);
		} else if ((!xmlStrcmp(sub->name, (const xmlChar *) "sinks"))) {
			parseSinks(doc, sub, conf);
		}
		sub = sub->next;
	}

	logfile.flush();
	xmlFreeDoc(doc);
	return 0;
}

