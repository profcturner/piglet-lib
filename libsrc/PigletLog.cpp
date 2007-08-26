/**
** Piglet Productions
**
** FileName       : PigletLog.cpp
**
** Implements     : PigletLog
**
** Description
**
** A FrontDoor style log class
**
**
** Initial Coding : Colin Turner
**
** Date           : Unknown
**
**
** Copyright applies on this file, and distribution may be limited.
** Copyright 1996-99 Colin Turner, distribution is limited
*/

/*
** Revision 1.10
**
** Modified for compatibility with Watcom and other operating systems.
** (Colin Turner)
**
*/

#include <PigletLog.h>

#if defined(__WATCOMC__)
#include <PigletCore.h>
#endif


/****************************************************************************/
/*                                                                          */
/*                                  g_Log                                   */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                     S E R V I C E   F U N C T I O N S                    */
/****************************************************************************/

/****************************************************************************/
/*                      P U B L I C   F U N C T I O N S                     */
/****************************************************************************/


void PigletLog::Printf(char *string, ...)
{
  va_list ap;
  va_start(ap, string);
  VPrintf(0, string, ap);
  va_end(ap);
}


void PigletLog::Printf(char preline, char *string, ...)
{
  char store=Preline;
  va_list ap;
  Preline=preline;
  va_start(ap, string);
  VPrintf(0, string, ap);
  va_end(ap);
  Preline=store;
}

void PigletLog::Debugf(int level, char *string, ...)
{
  va_list ap;
  va_start(ap, string);
  VPrintf(level, string, ap);
  va_end(ap);
}

void PigletLog::Debugf(int level, char preline, char *string, ...)
{
  char store=Preline;
  va_list ap;
  Preline=preline;
  va_start(ap, string);
  VPrintf(level, string, ap);
  va_end(ap);
  Preline=store;
}

void PigletLog::VPrintf(int level, char *string, va_list ap)
{
  // This function calls both EchoPrintf and fprintf as required.
  register int loop,loop2;
  char buffer[10];
  int logs=0, logf=0;

  // Storage space
  char sc; signed int sd; unsigned int su; char *ss;

  unsigned long int sul; signed long int sdl;

  logs=(log!=NULL && level<=DebugLevel);
  logf=((Flags & PigletLog_EchoPrint) && level<=EchoLevel);
  if(!logs && !logf) return;

  // Run through the basic string
  for(loop=0; string[loop]!=0; loop++){
    switch (string[loop]){
      case '%':
        // Dealing with a format command, look for a variable
        // Check for %%
        if(string[loop+1]=='%'){
          if(logs) fprintf(log,"%%");
          if(logf) EchoPrintf("%%");
        }
        else{
          // Scan to the next alphabetic or zero char
          for(loop2=loop+1; !IsSpecial(string[loop2]) && string[loop2]; loop2++);

          strncpy(buffer, string+loop, loop2-loop+2);
          switch (string[loop2]){
            case 'l'  :
              buffer[loop2-loop+2]=0;
              // great :-(, long variable
              loop2++;
              switch (string[loop2]){
                case 'd' :
                case 'i' :
                  // long signed decimal
                  sdl = va_arg(ap, signed long);
                  if(logs) fprintf(log, buffer, sdl);
                  if(logf) EchoPrintf(buffer, sdl);
                  break;
                case 'u' :
                  // long unsigned integer
                  sul = va_arg(ap, unsigned long);
                  if(logs) fprintf(log, buffer, sul);
                  if(logf) EchoPrintf(buffer, sul);
                  break;
                case 'X' :
                case 'x' :
                  // long hexadesimal
                  sul = va_arg(ap, unsigned long);
                  if(logs) fprintf(log, buffer, sul);
                  if(logf) EchoPrintf(buffer, sul);
                  break;
              }
            case '\0' :
              // Unexpected end of string!
              break;
            case 'c' :
              // character
              buffer[loop2-loop+1]=0;
              sc = va_arg(ap, char);
              if(logs) fprintf(log,buffer, sc);
              if(logf) EchoPrintf(buffer, sc);
              break;
            case 'd' :
            case 'i' :
              // signed decimal integer
              buffer[loop2-loop+1]=0;
              sd = va_arg(ap, signed);
              if(logs) fprintf(log,buffer, sd);
              if(logf) EchoPrintf(buffer, sd);
              break;
            case 's' :
              // string
              buffer[loop2-loop+1]=0;
              ss = va_arg(ap, char *);
              if(logs) fprintf(log,buffer, ss);
              if(logf) EchoPrintf(buffer, ss);
              break;
            case 'u' :
              // unsigned integer
              buffer[loop2-loop+1]=0;
              su = va_arg(ap, unsigned);
              if(logs) fprintf(log,buffer, su);
              if(logf) EchoPrintf(buffer, su);
              break;
            case 't' :
              // time in log format
              StartLine(level);
              break;
            //#ifdef g_RunDebug
            case 'm' :
            // coreleft, try to preserve formatting info as much as possible.
              buffer[loop2-loop+1]=0;
              strcpy(buffer+(strlen(buffer)-1), "lu");
              #ifdef __WATCOMC__
              if(logs) fprintf(log, buffer, _memavl());
              if(logf) EchoPrintf(buffer, _memavl());
              #else
              if(logs) fprintf(log, buffer, coreleft());
              if(logf) EchoPrintf(buffer, coreleft());
              #endif
              break;
            case 'z' :
              // stackleft, debug only, try to preserve formatting.
              buffer[loop2-loop+1]=0;
              strcpy(buffer+(strlen(buffer)-1), "lu");
              if(logs) fprintf(log, buffer, stackavail());
              if(logf) EchoPrintf(buffer, stackavail());
              break;
            case 'X' :
            case 'x' :
              // Hexadecimal digits
              buffer[loop2-loop+1]=0;
              su = va_arg(ap, unsigned);
              if(logs) fprintf(log, buffer, su);
              if(logf) EchoPrintf(buffer, su);
              break;

          }
        }
        loop=loop2;
        break;
      case '\\':
        // Dealing with a special character
        // send next two chars
        strncpy(buffer, string+loop, 2);
        buffer[2]=0;
        if(logs) fprintf(log,"%s",buffer);
        if(logf) EchoPrintf("%s", buffer);
        break;
      default :
        // Bog standard char :-) */
        if(logs) fprintf(log,"%c",string[loop]);
        if(logf) EchoPrintf("%c", string[loop]);
        break;
    }
  }
  if(Flags & PigletLog_FlushEvery) Flush();
}


void PigletLog::Finish()
{
  int EchoOn=(int) (Flags & PigletLog_EchoPrint);
  if(log){
    if(EchoOn) Flags = Flags & ~PigletLog_EchoPrint;
    StartLine(0);
    fprintf(log, "%s Normal Termination\n", Name);
    StartLine(0);
    fprintf(log, "Execution time %.02f seconds\n", Extime());
    fclose(log);
    if(EchoOn) Flags = Flags | PigletLog_EchoPrint;
  }
}

void PigletLog::Abnormal()
{
  int EchoOn=(int) (Flags & PigletLog_EchoPrint);
  if(log){
    if(EchoOn) Flags = Flags & ~PigletLog_EchoPrint;
    StartLine(0);
    fprintf(log, "%s Abnormal Termination\n", Name);
    StartLine(0);
    fprintf(log, "Execution time %.02f seconds\n", Extime());
    fclose(log);
    if(EchoOn) Flags = Flags | PigletLog_EchoPrint;
  }
}

void PigletLog::Flush()
{
  if(log) fflush(log);
}

/****************************************************************************/
/*                     P R I V A T E  F U N C T I O N S                     */
/****************************************************************************/


void PigletLog::EchoPrintf(char *string, ...)
{
  va_list ap;
  va_start(ap, string);
  if(Flags & PigletLog_EchoPrint) vprintf(string, ap);
  va_end(ap);

}


float PigletLog::Extime(void)
{
  clock_t temp;
  float running;

  temp = clock();
  running = temp/CLK_TCK;

  return((float) running);
}

// No check is made on validity of output file. This function places the
// time (correctly formated for country codes) with the preline
// character, to file.
void PigletLog::StartLine(int level)
{
  struct tm *local;
  time_t t;

  t = time(NULL);
  local = localtime(&t);
  if(log && level<=DebugLevel) fprintf(log,"%c %2u%s%02u%s%02u  ", Preline, local->tm_hour, CoTSep, local->tm_min, CoTSep, local->tm_sec);
  if((Flags & PigletLog_EchoPrint) && (Flags & PigletLog_EchoTime) && level<=EchoLevel) EchoPrintf("%c %2u%s%02u%s%02u  ", Preline, local->tm_hour, CoTSep, local->tm_min, CoTSep, local->tm_sec);
}

/****************************************************************************/
/*             I M P L E M E N T A T I O N   F U N C T I O N S              */
/****************************************************************************/

/****************************************************************************/
/*                      P U B L I C   F U N C T I O N S                     */
/****************************************************************************/


PigletLog::PigletLog()
{
  Constructor("", "", "", 0);
}

PigletLog::PigletLog(FILE *fp, char *name, char *version, char *extra, long flags)
{
  Constructor(name, version, extra, flags);
  Attach(fp);
}

PigletLog::PigletLog(char *filename, char *name, char *version, char *extra, long flags)
{
  Constructor(name, version, extra, flags);
  SetFile(filename);
  Open();
}


/****************************************************************************/
/*                     P R I V A T E  F U N C T I O N S                     */
/****************************************************************************/

void PigletLog::Constructor(char *name, char *version, char *extra, long flags)
{
  strcpy(Name, name);
  strcpy(Version, version);
  strcpy(Extra, extra);
  Flags=flags;
  Preline=0x20;
  DebugLevel=0;
  strcpy(CoDSep,"/");
  strcpy(CoTSep,":");
  GetCountry();
  log=NULL;
}

void PigletLog::GetCountry(void)
{
  // A function to grab country configuration
  struct COUNTRY space;
  struct COUNTRY * pointer = &space;

  #ifdef __WATCOMC__
  PigletGetCountry(&space);
  #else
  pointer=country(0, pointer);
  #endif
  

  if(pointer!=NULL){
    DateFormat=pointer->co_date;
    strcpy(CoDSep,pointer->co_dtsep);
    strcpy(CoTSep,pointer->co_tmsep);
  }
}


int PigletLog::Open()
{

  log=_fsopen(Filename, "a", SH_DENYWR);
  if(!log) return(0);
  Start();
  return(1);
}


void PigletLog::Start(void)
{
  struct tm *local;
  time_t t;
  char stamp[20];

  if(!log) return;
  t=time(NULL);
  local=localtime(&t);
  strftime(stamp,20,"  %a %d %b %y",local);
  fprintf(log, "\n----------%s, %s, Version %s; %s\n", stamp, Name, Version, Extra);


}

int PigletLog::IsSpecial(char test)
{
  if(test=='c' || test=='d' || test=='i' || test=='s' || test=='f' || test=='u' || test=='t' || test=='x' || test=='X' || test=='l' || test=='m' || test=='z') return(1);
  return(0);
}
