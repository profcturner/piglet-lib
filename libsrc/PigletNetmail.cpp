/**
** Piglet Productions
**
** FileName       : PigletNetmail.cpp
**
** Implements     : PigletNetmail
**
** Description
**
** This is a class designed for easy handling of FTS1 (.MSG) netmail messages.
**
** Initial Coding : Colin Turner
**
** Date           : 1996
**
**
** Copyright applies on this file, and distribution may be limited.
** Copyright Colin Turner, All Rights Reserved, Distribution is limited.
*/

/*
** Revision 1.00
**
*/



#include <PigletNetmail.h>
#include <PigletCore.h>

//define g_RunDebug_nm

#ifdef PigletRunDebug_NetMail
#include <PigletLog.h>
extern class PigletLog Trace;
#endif

static char *PigletNMMonthName[]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};


/**************************************************************************/
/*                                                                        */
/*                      I M P L E M E N T A T I O N                       */
/*                                                                        */
/**************************************************************************/

PigletNetmail::PigletNetmail()
{
  Constructor("", NULL, 0);
}


PigletNetmail::PigletNetmail(char *filename, PigletMsgId * Id)
{
  Constructor(filename, Id, 0);
}


PigletNetmail::PigletNetmail(char *filename, PigletMsgId * Id, long flags)
{
  Constructor(filename, Id, flags);
}


void PigletNetmail::Constructor(char *filename, PigletMsgId * Id, long flags)
{
  // clean all the fields
  memset(this, 0, sizeof(PigletNetmail));
  strcpy(FileName, filename);
  IdObject = Id;
  ClassFlags = flags;
  fp=NULL;

  // If it's sensible, load the header data
  if(strlen(FileName) && !(PigletNMNoAutoLoad & ClassFlags)) LoadHeader();
}


PigletNetmail::~PigletNetmail()
{
  // Deallocate storage
  SetString(&Msgid, "");
  SetString(&Reply, "");
  SetString(&ReplyTo, "");
  SetString(&ReplyAddr, "");
  SetString(&IntTo, "");
  SetString(&IntFrom, "");
  SetString(&Flags, "");
  SetString(&Text, "");
  SetString(&PID, "");
  if(fp) fclose(fp);
}


void PigletNetmail::SetString(char ** toset, char *value)
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


char *PigletNetmail::GetString(char *toget)
{
  return(toget); // pretty basic, at least for now.
}


int PigletNetmail::LoadHeader()
{
  // Returns 0 : OK
  //         1 : Parse / eof error
  //         2 : Unable to open file error
  char *KludgeBuffer = new char[256]; // maximum size of kludges
  int  AfterKludge=0;
  char test=0;
  char * pointer;

  #ifdef PigletRunDebug_NetMail
  Trace.Printf("%tg_Nm::LoadHeader()\n");
  #endif

  if(!fp) fp=_fsopen(FileName, "rb", SH_DENYWR);
  if(!fp){
   delete KludgeBuffer;
   return(2);
  }

  #ifdef PigletRunDebug_NetMail
  Trace.Printf("%tg_Nm::LoadHeader : Load static header\n");
  #endif
  // Read in static FTS header
  fseek(fp, 0, SEEK_SET);
  fread(this, (size_t) PigletNMMessageHeader ,1 ,fp);

  #ifdef PigletRunDebug_NetMail
  Trace.Printf("%tg_Nm::LoadHeader : Read other control data (Kludges)\n");
  #endif
  // Read in control kludges
  while(!feof(fp) && !AfterKludge){
   do{
    test=fgetc(fp);
   }
   while(!feof(fp) && test==0x0A);
   if(test!=1){
    // Oops, not a kludge!
    fseek(fp, -1L, SEEK_CUR); // rewind over the incorrect character
    AfterKludge=1;
   }
   else{
    ReadToCR(fp, KludgeBuffer, 255);
    pointer=KludgeBuffer;
    // Now check this captured kludge against the ones we "need", and
    // process those correctly
    if(!strncmp(KludgeBuffer, "INTL", 4)){
      // Process INTL kludge
      pointer=KludgeBuffer+4;
      if(*pointer==' ') while(*pointer==' ') pointer++;
      DestZone=atoi(pointer);
      OrigZone=atoi(strpbrk(pointer, " ")+1);
    }
    if(!strncmp(KludgeBuffer, "FMPT", 4)){
      // Process FMPT kludge
      pointer=KludgeBuffer+4;
      if(*pointer==' ') while(*pointer==' ') pointer++;
      OrigPoint=atoi(pointer);
    }
    if(!strncmp(KludgeBuffer, "TOPT", 4)){
      // Process TOPT kludge
      pointer=KludgeBuffer+4;
      if(*pointer==' ') while(*pointer==' ') pointer++;
      DestPoint=atoi(pointer);
    }
    if(!strncmp(KludgeBuffer, "MSGID:", 6)){
      // Process MSGID kludge
      pointer=KludgeBuffer+6;
      if(*pointer==' ') while(*pointer==' ') pointer++;
      SetString(&Msgid, pointer);
    }
    if(!strncmp(KludgeBuffer, "REPLY:", 6)){
      // Process REPLY kludge
      pointer=KludgeBuffer+6;
      if(*pointer==' ') while(*pointer==' ') pointer++;
      SetString(&Reply, pointer);
    }
    if(!strncmp(KludgeBuffer, "REPLYTO", 7)){
      // Process REPLYTO kludge
      pointer=KludgeBuffer+7;
      if(*pointer==' ') while(*pointer==' ') pointer++;
      SetString(&ReplyTo, pointer);
    }
    if(!strncmp(KludgeBuffer, "REPLYADDR", 9)){
      // Process REPLYADDR kludge
      pointer=KludgeBuffer+9;
      if(*pointer==' ') while(*pointer==' ') pointer++;
      SetString(&ReplyAddr, pointer);
    }
    if(!strncmp(KludgeBuffer, "From:", 5)){
      // Process From: (GIGO) kludge
      pointer=KludgeBuffer+5;
      if(*pointer==' ') while(*pointer==' ') pointer++;
      SetString(&IntFrom, pointer);
    }
    if(!strncmp(KludgeBuffer, "FLAGS", 5)){
      // Process FLAGS kludge
      pointer=KludgeBuffer+5;
      if(*pointer==' ') while(*pointer==' ') pointer++;
      SetString(&Flags, pointer);
    }
    if(!strncmp(KludgeBuffer, "PID:", 4)){
      // Process PID kludge
      pointer=KludgeBuffer+4;
      if(*pointer==' ') while(*pointer==' ') pointer++;
      SetString(&PID, pointer);
    }

   }
  }
  TextStart = ftell(fp);

  #ifdef PigletRunDebug_NetMail
  Trace.Printf("%tg_Nm::LoadHeader : Internet relevant text lines\n");
  #endif
  // Check for a "To:" line at the top of the text (first paragraph)
  do{
   ReadToCR(fp, KludgeBuffer, 255);
   if(!strnicmp(KludgeBuffer, "To:", 3)){
    pointer=KludgeBuffer+3;
    if(*pointer==' ') while(*pointer==' ') pointer++;
    SetString(&IntTo, pointer);
   }
   if(!strnicmp(KludgeBuffer, "From:", 5)){
    pointer=KludgeBuffer+5;
    if(*pointer==' ') while(*pointer==' ') pointer++;
    SetString(&IntFrom, pointer);
   }
  } while(*KludgeBuffer);

  if(ClassFlags & PigletNMKeepClosed) fclose(fp);
  delete KludgeBuffer;
  return(1);
}


char * PigletNetmail::GetKludge(char * kludge, char * buffer)
{
  *buffer=0;
  int AfterKludge=0;
  char test;
  char * pointer;
  char * KludgeBuffer = new char[256]; // maximum size of kludges

  #ifdef PigletRunDebug_NetMail
  Trace.Printf("%tPigletNetmail::GetKludge(%s,...)\n", kludge);
  #endif
  if(!fp) fp=_fsopen(FileName, "rb", SH_DENYWR);
  if(fp)
  {
   fseek(fp, (size_t) PigletNMMessageHeader, SEEK_SET);
   while(!feof(fp) && !AfterKludge){
    test=fgetc(fp);
    if(test!=1){
      // Oops, not a kludge!
      fseek(fp, -1L, SEEK_CUR); // rewind over the incorrect character
      AfterKludge=1;
    }
    else{
      ReadToCR(fp, KludgeBuffer, 255);
      #ifdef g_RunDebug_nm
      Trace.Printf("%tPigletNetmail::GetKludge : [%s]\n", KludgeBuffer);
      #endif
      pointer=KludgeBuffer;
      // Now check this captured kludge against the ones we "need", and
      // process those correctly
      if(!strnicmp(KludgeBuffer, kludge, strlen(kludge))){
       pointer=KludgeBuffer+=strlen(kludge);
       while(*pointer==' ') pointer++;
       strcpy(buffer, pointer);
       AfterKludge=1;
      }
    }
   }
  }
  fseek(fp, (size_t) TextStart, SEEK_SET);
  delete KludgeBuffer;
  return(buffer);
}


int PigletNetmail::ReadToCR(FILE *file, char *buffer, size_t maxlength)
{
  // This functions reads from a file, placing input into the buffer until
  // either it meets a CR character, or the end of the buffer. In the latter
  // case, the file pointer will be left after the next CR character even though
  // the read is not complete.

  // Returns 0 : OK
  // Returns 1 : Kludge not completely read
  unsigned short int index=0;
  int quit=0;
  int error=0;
  char temp=0;

  if(feof(file)){
   *buffer=0;
   return(1);
  }
  while(!feof(file) && !quit && index<maxlength){
   temp=fgetc(file);
   if(temp==0 || temp=='\r' || temp=='\n'){
    buffer[index]=0;
    quit=1;
   }
   else{
    if(temp!='\n' && temp!='\r'){
      buffer[index]=temp;
      index++;
    }
   }
  }
  if(!quit && !feof(file)){
   // We didn't hit the CR, nor the end of the file, we better look for it.
   buffer[maxlength-1]=0; // Make the partially read string safe.
   while(!feof(file) && !quit){
    temp=fgetc(file);
    if(temp=='\n' || temp=='\r') quit=1;
   }
   error=1;
   *buffer=0;
  }
  return(error);
}


int PigletNetmail::OpenMsg()
{
  fp=_fsopen(FileName, "wb", SH_DENYRW);
  return(fp!=NULL);
}


long PigletNetmail::OpenMsg(char *path, long highest)
{
  // Attempts to open a netmail <highest>.MSG in the defined path
  // returns the new hightest

  // You MUST check that the path exists and is completed with a backslash
  int success=0;

  while(!success){
   sprintf(FileName, "%s%lu.MSG", path, highest);
   if(!CheckFile(FileName)){
    fp=_fsopen(FileName, "wb", SH_DENYRW);
    if(fp) success=1;
   }
   highest++;
  }
  return(highest);
}


int PigletNetmail::SaveHeader()
{
  return(SaveHeader(0));
}


int PigletNetmail::SaveHeader(time_t oldtime)
{
  // Save header information ready for use kludges and text
  // Uses time in oldtime, or current if oldtime = 0

  // returns
  // 0 on failure
  // 1 on success

  #ifdef PigletRunDebug_NetMail
  Trace.Printf("%tg_Nm::SaveHeader()\n");
  #endif
  time_t t;
  struct tm *local;

  // write time info (binary)
  if(!oldtime) t=time(NULL); else t=oldtime;
  DateWritten = (unsigned long) t;

  // Forms the ASCII time string required for the .MSG format
  local=localtime(&t);
  sprintf(Date, "%02u %s %02u %02u:%02u:%02u", local->tm_mday, PigletNMMonthName[local->tm_mon], local->tm_year % 100, local->tm_hour, local->tm_min, local->tm_sec);

  // Go to start
  if(!fp) OpenMsg();
  if(!fp) return(0);
  fseek(fp, 0, SEEK_SET);

  // Write the binary header
  fwrite(this, (time_t) PigletNMMessageHeader, 1, fp);

  // Now control kludges
  fprintf(fp, "%cINTL %u:%u/%u %u:%u/%u\r", 1, DestZone, DestNet, DestNode, OrigZone, OrigNet, OrigNode);
  if(DestPoint)  fprintf(fp, "%cTOPT %u\r", 1, DestPoint);
  if(OrigPoint)  fprintf(fp, "%cFMPT %u\r", 1, OrigPoint);
  if(strlen(Flags))     fprintf(fp, "%cFLAGS %s\r", 1, Flags);
  if(strlen(Msgid))     fprintf(fp, "%cMSGID: %s\r", 1, Msgid);
  if(strlen(Reply))     fprintf(fp, "%cREPLY: %s\r", 1, Reply);
  if(strlen(ReplyTo))   fprintf(fp, "%cREPLYTO %s\r", 1, ReplyTo);
  if(strlen(ReplyAddr)) fprintf(fp, "%cREPLYADDR %s\r", 1, ReplyAddr);
  if(strlen(PID))       fprintf(fp, "%cPID: %s\r", 1, PID);

  // Any special environment kludges
  if(getenv("TZUTC")) fprintf(fp, "%cTZUTC: %s\r", 1, getenv("TZUTC"));

  return(1);

}


int PigletNetmail::Remove()
{
  #ifdef PigletRunDebug_NetMail
  Trace.Printf("%tg_Nm:Remove()\n");
  #endif
  if(fp) fclose(fp);
  return(remove(FileName));
}


int PigletNetmail::Receive()
{
  #ifdef PigletRunDebug_NetMail
  Trace.Printf("%tg_Nm::Receive()\n");
  #endif
  if(fp) fclose(fp);
  fp=_fsopen(FileName, "r+b", SH_DENYRW);
  if(!fp) return(0);
  fread(this, (size_t) PigletNMMessageHeader, 1, fp);
  Attr |= 0x0004;
  fseek(fp, 0, SEEK_SET);
  fwrite(this, (size_t) PigletNMMessageHeader, 1, fp);
  fclose(fp);
  return(1);
}


void PigletNetmail::Close()
{
  #ifdef PigletRunDebug_NetMail
  Trace.Printf("%tg_Nm::Close()\n");
  #endif
  if(fp) fclose(fp);
  fp=NULL;
}


void PigletNetmail::CloseNull()
{
  #ifdef PigletRunDebug_NetMail
  Trace.Printf("%tg_Nm::CloseNull()\n");
  #endif
  if(fp){
   fputc(0, fp);
   fclose(fp);
  }
  fp=NULL;
}

