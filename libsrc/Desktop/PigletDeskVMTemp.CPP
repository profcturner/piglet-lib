/*
** Piglet Productions
**
** FileName       : PigletDeskVMTemp.CPP
**
** Implements     : PigletVBarMenu (Template Loading)
**                  PigletVMenuTemplate
**
** Description
**
** Allows menu data to be read from files.
**
**
** Initial Coding : Colin Turner
**
** Date           : 1998
**
*/

#include <PigletDesktop.h>


/*
**    LoadTemplate
**
** This function should read template data in a formatted file straight into
** the menu data.
**
**    Parameters
**
**    tp     (Open) file handle of the data file containing the template
**    ident  Unique case sensitive text identifier for template block
**
**    Returns
**
**    1 on success, 0 on failure
**/
int PigletVBarMenu::LoadTemplate(FILE * tp, char * Ident)
{
  int               found = 0;
  int               error;
  unsigned short    loop;
  PigletVMenuTemplate * MenuHdr = new PigletVMenuTemplate;
  char *            Data;
  char *            Status;

  fseek(tp, 0, SEEK_SET);

  while (!found && !feof(tp)) {
    if (fread(MenuHdr, sizeof(PigletVMenuTemplate), 1, tp)==1) {
      if (strcmp(Ident, MenuHdr->Ident)) // No match, read past entries
        fseek(tp, (unsigned long) ((MenuHdr->DataLen + MenuHdr->StatusLen)*MenuHdr->Items), SEEK_CUR);
      else {
        X = MenuHdr->xOffset;
        Y = MenuHdr->yOffset;
        Data   = new char[MenuHdr->DataLen];
        Status = new char[MenuHdr->StatusLen];
        for (loop=0; loop<MenuHdr->Items; loop++) {
          error = 0;
          if (fread(Data, MenuHdr->DataLen, 1, tp)!=1) error=1;
          if (fread(Status, MenuHdr->StatusLen, 1, tp)!=1) error=1;
          if (!error) Add(Data, Status);
        }
        delete Status;
        delete Data;
        found = 1;
      }
    }
  }
  delete(MenuHdr);
  return(found);
}


// End of File
