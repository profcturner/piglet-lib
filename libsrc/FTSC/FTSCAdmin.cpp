



#define FTSC_AuthorData    "author.dat"
#define FTSC_AuthorDataTMP "author.tmp"
#define FTSC_AuthorDataSem "author.bsy"





int FillAuthor(FTSCAuthor * ToFill, char * Name, char * Address, char * Email, char * WWW, char * Company)
{
  strcpy(ToFill->Name, Name);
  ToFill->Address.ConvertFromString(Address);
  strcpy(ToFill->Email, Email);
  strcpy(ToFill->WWW, WWW);
  strcpy(ToFill->Company, Company);

  return(1);
}


int AddAuthor(FTSCAuthor & NewAuthor)
{
  unsigned long highest = 1; // highest unallocated code
  int present = 0;           // Is the author already on file
  FILE * infile;
  FILE * outfile;
  FTSCAuthor CurAuthor;

  infile  = RetryOpen(FTSC_AuthorData, "rb");
  outfile = RetryOpen(FTSC_AuthorDataTMP, "wb");

  while(!feof(infile)){
    CurAuthor.Load(infile);
    present = (CurAuthor == NewAuthor);
    if(CurAuthor.AuthorID >= highest) highest = CurAuthor.AuthorID + 1;
    CurAuthor.Save(outfile);
  }
  if(!present){
    NewAuthor.AuthorID = highest;
    NewAuthor.Save(outfile);
  }

  fclose(outfile);
  fclose(infile);

  if(!present){
    OpSysFile.Delete(FTSC_AuthorData);
    OpSysFile.Rename(FTSC_AuthorDataTMP);
    return(1);
  }
  
  OpSysFile.Delete(FTSC_AuthorDataTMP);
  return(0);
}
