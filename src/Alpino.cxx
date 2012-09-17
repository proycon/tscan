/*
  $Id: tscan.cxx 15187 2012-09-17 09:12:13Z sloot $
  $URL: https://ilk.uvt.nl/svn/sources/tscan/trunk/src/tscan.cxx $

  Copyright (c) 1998 - 2012
 
  This file is part of tscan

  tscan is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  tscan is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, see <http://www.gnu.org/licenses/>.

  For questions and suggestions, see:
      
  or send mail to:
      
*/

#include <cstdio> // for remove()
#include <cstring> // for strerror()
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <iostream>
#include <fstream>
#include "config.h"
#include "libfolia/folia.h"
#include "libfolia/document.h"
#include "tscan/Alpino.h"

using namespace std;
//using namespace TiCC;
using namespace folia;

void addSU( xmlNode *n, vector<Word*>& words, FoliaElement *s ){
  if ( Name( n ) == "node" ){
    KWargs atts = getAttributes( n );
    string cls = atts["cat"];
    bool leaf = false;
    if ( cls.empty() ){
      cls = atts["lcat"];
      leaf = true;
    }
    if ( !cls.empty() ){
      FoliaElement *e = 
	s->append( new SyntacticUnit( s->doc(), "cls='" + cls + "'" ) );
      if ( leaf ){
	string posS = atts["begin"];
	int start = stringTo<int>( posS );
	e->append( words[start] );
      }
      else {
	xmlNode *pnt = n->children;
	while ( pnt ){
	  addSU( pnt, words, e );
	  pnt = pnt->next;
	}
      }
    }
  }
}

void extractSyntax( xmlNode *node, folia::Sentence *s ){
  Document *doc = s->doc();
  doc->declare( AnnotationType::SYNTAX, 
		"myset", 
		"annotator='alpino'" );
  vector<Word*> words = s->words();
  FoliaElement *layer = s->append( new SyntaxLayer( doc ) );
  FoliaElement *sent = layer->append( new SyntacticUnit( doc, "cls='s'" ) );
  xmlNode *pnt = node->children;
  while ( pnt ){
    addSU( pnt, words, sent );
    pnt = pnt->next;
  }
}

void extractDependency( xmlNode *node, folia::Sentence *s ){
}

void extractAndAppendParse( xmlDoc *doc, folia::Sentence *s ){
  cerr << "extract the Alpino results!" << endl;
  xmlNode *root = xmlDocGetRootElement( doc );
  xmlNode *pnt = root->children;
  while ( pnt ){
    if ( folia::Name( pnt ) == "node" ){
      cerr << "founds a node" << endl;
      // 1 top node i hope
      extractSyntax( pnt, s );
      cerr << "added syntax " << s->xmlstring() << endl;
      extractDependency( pnt, s );
      break;
    }
    pnt = pnt->next;
  }
}

bool AlpinoParse( folia::Sentence *s ){
  string txt = folia::UnicodeToUTF8(s->text());
  cerr << "parse line: " << txt << endl;
  struct stat sbuf;
  int res = stat( "/tmp/alpino", &sbuf );
  if ( !S_ISDIR(sbuf.st_mode) ){
    res = mkdir( "/tmp/alpino/", S_IRWXU|S_IRWXG );
  }
  res = stat( "/tmp/alpino/parses", &sbuf );
  if ( !S_ISDIR(sbuf.st_mode) ){
    res = mkdir( "/tmp/alpino/parses",  S_IRWXU|S_IRWXG );
  }
  ofstream os( "/tmp/alpino/tempparse.txt" );
  os << txt;
  os.close();
  string parseCmd = "Alpino -fast -flag treebank /tmp/alpino/parses end_hook=xml -parse < /tmp/alpino/tempparse.txt -notk > /dev/null 2>&1";
  res = system( parseCmd.c_str() );
  cerr << "parse res: " << res << endl;
  remove( "/tmp/alpino/tempparse.txt" );
  xmlDoc *xmldoc = xmlReadFile( "/tmp/alpino/parses/1.xml", 0, XML_PARSE_NOBLANKS );
  if ( xmldoc ){
    extractAndAppendParse( xmldoc, s );
  }
  return (xmldoc != 0 );
}

