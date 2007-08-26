/**
** Piglet Productions
**
** FileName       : PigletHTML.cpp
**
** Implements     : PigletHTML
**
** Description
**
** Implements a class for easier output to HTML files.
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

#include <PigletHTML.h>


/**
**    WriteHeader
**
** This virtual function writes the header of the HTML page. Usually any
** inherited version should call this base first.
**
**/
virtual void PigletHTML::WriteHeader()
{
  if(!Open) return;

  fprintf(File, "<HTML>\n");
  fprintf(File, "<HEAD>\n");
  if(CSS)   fprintf(File, "<LINK REL=\"STYLESHEET\" TYPE=\"TEXT/CSS\" HREF=\"%s\">", CSS);
  if(Title) fprintf(File, "<TITLE>%s</TITLE>", Title);
  if(Description) Meta("description", Description);
  if(Keywords)    Meta("keywords", Keywords);
  fprintf(File, "</HEAD>\n");
  fprintf(File, "<BODY");
  
  if(BackGroundImage) fprintf(File, " BACKGROUND=\"%s\">", BackGroundImage);
  else fprintf(File, ">");
}



/**
**    WriteFooter
**      
** Virtual function which writes footer information
** to the web page. Any derived class should usually
** call this function after the derived version.
**
**/
virtual void PigletHTML::WriteFooter()
{
  if(!Open) return;

  fprintf(File, "</BODY>\n");
  fprintf(File, "</HTML>\n");
}


/**
**    WriteURL
**
** Writes a URL, both the text and the appropriate link.
**
**    Parameters
**
**    text    The text to write on screen
**    link    The link for the text
**
**/
void PigletHTML::WriteURL(const char * text, const char * link)
{
  if(!Open) return;
  
  fprintf(File, "<A HREF=\"%s\">%s</A>", link, text);
}


/**
**    HR
**
** Writes a horizontal rule to the page. This function is virtual
** allowing an image to be inserted instead.
**
**/
virtual void PigletHTML::HR()
{
  if(!Open) return;

  fprintf(File, "\n<HR>\n");
}


/**
**    InsertImage
**
** Inserts an image into the HTML document.
**
**    Parameters
**
**    SourceName  This is the URL for the source image
**    AltName     The alternative (textual) name - compulsory in HTML 4
**    height      Height to speed rendering, zero removes tag
**    width       Width to speed rendering, zero removes tag
**
**/
void PigletHTML::InsertImage(const char * SourceName, const char * AltName, unsigned short height, unsigned short width)
{
  InsertImage(SourceName, AltName, NULL, NULL, height, width);
}


/**
**    InsertImage
**
** Inserts an image into the HTML document with support for a hyperlink.
**
**    Parameters
**
**    SourceName  This is the URL for the source image
**    AltName     The alternative (textual) name - compulsory in HTML 4
**    Link        Hyperlink for the image if needed (NULL removes)
**    Align       Type of alignment (NULL removes)
**    height      Height to speed rendering, zero removes tag
**    width       Width to speed rendering, zero removes tag
**
**/
void PigletHTML::InsertImage(const char * SourceName, const char * AltName, const char * Link, const char * Align, unsigned short height, unsigned short width)
{
  if(!Open) return;

  if(Link)   fprintf(File, "<A HREF = \"%s\">", Link);
             fprintf(File, "<IMG SRC=\"%s\" ALT=\"%s\"", SourceName, AltName);
  if(Align)  fprintf(File, " ALIGN=\"%s\"", Align);
  if(height) fprintf(File, " HEIGHT=\"%u\"", height);
  if(width)  fprintf(File, " WIDTH=\"%u\"", width);
             fprintf(File, ">");
  if(Link)   fprintf(File, "</A>");
             fprintf(File, "\n");
}


void PigletHTML::StartOrderedList()
{
  if(!Open) return;
  
  fprintf(File, "<OL>\n");
  OrderListDepth++;
}


void PigletHTML::EndOrderedList()
{
  if(!Open) return;
    
  fprintf(File, "</OL>\n");
  OrderListDepth--;
}


void PigletHTML::StartList()
{
  if(!Open) return;

  fprintf(File, "<UL>\n");
  UnOrderListDepth++;
}


void PigletHTML::EndList()
{
  if(!Open) return;

  fprintf(File, "</UL>\n");
  UnOrderListDepth--;
}


/**
**    ListItem
**
** This function is used to write an item into a list.
**
**    Parameters
**
**    item    The text of the item to write
**/
virtual void PigletHTML::ListItem(const char * item)
{
  int indent = (OrderListDepth + UnOrderListDepth) * 2;
  int loop;

  if(!Open) return;

  for(loop = 0; loop < indent; loop++) fputc(' ', File);

  fprintf(File, "<LI>%s</LI>", item);
}





void PigletHTML::CentreOn()
{
  if(!Open) return;
  
  fprintf(File, "<CENTER>");
}


void PigletHTML::CentreOff()
{
  if(!Open) return;
  
  fprintf(File, "</CENTER>");
}


void PigletHTML::BoldOn()
{
  if(!Open) return;
  
  fprintf(File, "<B>");
}


void PigletHTML::BoldOff()
{
  if(!Open) return;

  fprintf(File, "</B>");
}


virtual void PigletHTML::SignalError(int newError)
{
  Error = newError;
}

void PigletHTML::ClearError()
{
  Error = 0;
}

int PigletHTML::GetError()
{
  return(Error);
}


int PigletHTML::SetFile(FILE * connect)
{
  if(!Open){
    File = connect;
    Open = 1;
    return(1);
  }
  return(0);
}


int PigletHTML::SetFile(const char * filename)
{
  if(!Open){
    File = fopen(filename, "w");
    if(File){
      Open = 1;
      return(1);
    }
  }
  return(0);
}

PigletHTML::PigletHTML(FILE * connect)
{
  Constructor();
  SetFile(connect);
}


PigletHTML::PigletHTML(const char * filename)
{
  Constructor();
  SetFile(filename);
}


void PigletHTML::Constructor()
{
  CSS = NULL;
  Author = NULL;
  Title = NULL;
  Description = NULL;
  Keywords = NULL;
  BackGroundImage = NULL;

  Error = 0;
  Open = 0;

  OrderListDepth = 0;
  UnOrderListDepth = 0;
}


PigletHTML::~PigletHTML()
{
  SetString(&CSS, "");
  SetString(&Author, "");
  SetString(&Title, "");
  SetString(&Description, "");
  SetString(&Keywords, "");
  SetString(&BackGroundImage, "");
}


void PigletHTML::SetCSS(const char * newCSS)
{
  SetString(&CSS, newCSS);
}


void PigletHTML::SetAuthor(const char * newAuthor)
{
  SetString(&Author, newAuthor);
}


void PigletHTML::SetTitle(const char * newTitle)
{
  SetString(&Title, newTitle);
}


void PigletHTML::SetDescription(const char * newDescription)
{
  SetString(&Description, newDescription);
}


void PigletHTML::SetKeywords(const char * newKeywords)
{
  SetString(&Keywords, newKeywords);
}


void PigletHTML::SetBackGroundImage(const char * newBackGroundImage)
{
  SetString(&BackGroundImage, newBackGroundImage);
}


void PigletHTML::StartTable(const char * Align, int Pixels, int Columns)
{
  if(!Open) return;

  fprintf(File, "<TABLE");
  if(Align)   fprintf(File, " ALIGN=\"%s\"", Align);
  fprintf(File, " BORDER=\"%u\"", Pixels);
  if(Columns) fprintf(File, "COLS=\"%u\"", Columns);

  fprintf(File, ">\n");
}


void PigletHTML::EndTable()
{
  if(!Open) return;

  fprintf(File, "</TABLE>");
}


void PigletHTML::StartTableRow(const char * BGColour)
{
  if(!Open) return;

  fprintf(File, "<TR");
  if(BGColour) fprintf(File, "BGCOLOR=\"%s\"", BGColour);

  fprintf(File, ">\n");
}


void PigletHTML::EndTableRow()
{
  if(!Open) return;

  fprintf(File, "</TR>\n");
}


void PigletHTML::StartTableData()
{
  if(!Open) return;

  fprintf(File, "<TD>");
}


void PigletHTML::EndTableData()
{
  if(!Open) return;

  fprintf(File, "</TD>\n");
}


void PigletHTML::Printf( char *format, ... )
{
  va_list arglist;

  if(!Open) return;

  va_start( arglist, format );
  vfprintf(File, format, arglist );
  va_end( arglist );
}


void PigletHTML::SetString(char ** toset, const char * value)
{
  // This function is used to set, or reset the dynamic storage used in the
  // class.
  char *newvar=NULL;

  if(strlen(value)){
   newvar = new char[strlen(value)+1];
   if(!newvar) return; // Oops, memory allocation problem, abort.
   strcpy(newvar, value);
  }

  if(*toset){
   delete *toset;
   *toset=NULL;
  }
  *toset=newvar; // realign pointers
}

virtual void PigletHTML::Meta(char * name, char * contents)
{
  if(!Open) return;

  fprintf(File, "<META NAME=\"%s\" CONTENT=\"%s\">", name, contents);
}

