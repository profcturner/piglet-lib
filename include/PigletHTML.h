 /**
 ** Piglet Productions
 **
 ** FileName       : PigletHTML.h
 **
 ** Defines        : PigletHTML
 **
 ** Description
 **
 ** Allows more simple creation of HTML files, particularly for CGI
 ** output.
 **
 **
 ** Initial Coding : Colin Turner
 **
 ** Date           : 18th April 2000
 **
 **
 ** Copyright applies on this file, and distribution may be limited.
 */
 
 /*
 ** Revision 1.00
 **
 */
 
#ifndef Piglet_HTML_H
#define Piglet_HTML_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

class PigletHTML
{
  // Data
protected:
  FILE * File;
private:
  char * CSS;
  char * Author;
  char * Title;
  char * Description;
  char * Keywords;
  char * BackGroundImage;

  int Error;
  int Open;

  int OrderListDepth;
  int UnOrderListDepth;

  // Services
public:
  int SetFile(FILE * connect);
  int SetFile(const char * filename);

  void SetCSS(const char * newCSS);
  void SetAuthor(const char * newAuthor);
  void SetTitle(const char * newTitle);
  void SetDescription(const char * newDescription);
  void SetKeywords(const char * newKeywords);
  void SetBackGroundImage(const char * newBackGroundImage);

  // Direct HTML Services
  virtual void WriteHeader();
  virtual void WriteFooter();

  virtual void HR();

  void StartList();
  void EndList();
  void StartOrderedList();
  void EndOrderedList();
  virtual void ListItem(const char * item);

  // Appearance
  void CentreOn();
  void CentreOff();

  void BoldOn();
  void BoldOff();

  // Images
  void InsertImage(const char * SourceName, const char * AltName, const char * Link = NULL, const char * Align = NULL, unsigned short height = 0, unsigned short width = 0);
  void InsertImage(const char * SourceName, const char * AltName, unsigned short height, unsigned short width);

  // URLS
  void WriteURL(const char * text, const char * link);

  // Tables
  void StartTable(const char * Align, int Pixels, int Columns);
  void EndTable();
  void StartTableRow(const char * BGColour = NULL);
  void EndTableRow();
  void StartTableData();
  void EndTableData();

  // General
  void Printf(char * format, ...);


  virtual void SignalError(int newError);
  void ClearError();
  int  GetError();

  // Constructors and Destructors
  PigletHTML(FILE * connect = stdout);
  PigletHTML(const char * filename);
  virtual ~PigletHTML();

  // Implementation
private:
  void Constructor();
  virtual void Meta(char * name, char * contents);
protected:
  void SetString(char ** toset, const char * value);


  // Enumeration

};

#endif
