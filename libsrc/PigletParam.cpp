/**
** Piglet Productions
**
** FileName       : PigletParam.cpp
**
** Description
**
** Parameter parsing for command line and environment variables.
**
** Initial Coding : Colin Turner
**
** Date           : 1996
**
**
** Copyright applies on this file, and distribution may be limited.
** Copyright Colin Turner, All Rights Reserved, distribution is limited.
*/

/*
** Revision 1.00
**
*/


#include <PigletParam.h>

char ValidParamPrefix[]="/-";
char ValidParamSuffix[]=":=";


int ParamGetPath(char *parameter, char *path)
{
  /* A function to get the path to the program being called. This will */
  /* require a version of DOS past 3, well, tough, eh?                 */
  /* returns 0 on error, how?                */

  register int loop;
  /* rewind to the last backslash */
  /* Note the careful use of single quotes for the char, and the \\    */

  for(loop=strlen(parameter); (parameter[loop]!='\\') && (loop!=0); loop--);
  if(loop!=0){
    strncpy(path, parameter, loop+1);
    path[loop+2]=0;
  }
  return(loop);
}


int CheckParameter(char *parameter, char *test)
{
  if(strpbrk(parameter, ValidParamPrefix)!=parameter) return(0);
  if(!strcmp(parameter+1, test)) return(1); else return(0);
}


int CheckParameter(char *parameter, char *test, int *value, int mode)
{
  /* a general funtion to handle command line arguments, works like this */
  /* command line parameters are assumed to bo of the form :             */
  /* /test:value                                                         */
  /* so setup a loop which passes each parameter in in turn. In addition */
  /* the function will return                                            */
  /* 0 if no match is found;                                             */
  /* 1 if a match is found;                                              */

  switch(mode){
    case PARAMC:
    case PARAMCE:
      if(strpbrk(parameter, ValidParamPrefix)!=parameter) return(0);
      if(strncmp(parameter+1, test, strlen(test))) return(0);
      else{
        if(strpbrk(parameter+1+strlen(test), ValidParamSuffix)!=(parameter+1+strlen(test))) return(0);
        // We've now checked the prefix, test characters and suffix, success
        *value=atoi(parameter+2+strlen(test));
        return(1);
      }

    case PARAME:
      if(strnicmp(parameter, test, strlen(test))) return(0); // no match
      if(*(parameter+strlen(test))!='=') return(0); // no match
      *value=atoi(parameter+strlen(test)+1);
      return(1);
  }
  return(0); // huh?
}


int CheckParameter(char *parameter, char *test, long *value, int mode)
{
  /* a general funtion to handle command line arguments, works like this */
  /* command line parameters are assumed to bo of the form :             */
  /* /test:value                                                         */
  /* so setup a loop which passes each parameter in in turn. In addition */
  /* the function will return                                            */
  /* 0 if no match is found;                                             */
  /* 1 if a match is found;                                              */

  switch(mode){
    case PARAMC:
    case PARAMCE:
      if(strpbrk(parameter, ValidParamPrefix)!=parameter) return(0);
      if(strncmp(parameter+1, test, strlen(test))) return(0);
      else{
        if(strpbrk(parameter+1+strlen(test), ValidParamSuffix)!=(parameter+1+strlen(test))) return(0);
        // We've now checked the prefix, test characters and suffix, success
        *value=atol(parameter+2+strlen(test));
        return(1);
      }

    case PARAME:
      if(strnicmp(parameter, test, strlen(test))) return(0); // no match
      if(*(parameter+strlen(test))!='=') return(0); // no match
      *value=atol(parameter+strlen(test)+1);
      return(1);
  }
  return(0); // huh?
}


int CheckParameter(char *parameter, char *test, char *value, int mode)
{
  /* a general funtion to handle command line arguments, works like this */
  /* command line parameters are assumed to bo of the form :             */
  /* /test:value                                                         */
  /* so setup a loop which passes each parameter in in turn. In addition */
  /* the function will return                                            */
  /* 0 if no match is found;                                             */
  /* 1 if a match is found;                                              */

  switch(mode){
    case PARAMC:
    case PARAMCE:
      if(strpbrk(parameter, ValidParamPrefix)!=parameter) return(0);
      if(strncmp(parameter+1, test, strlen(test))) return(0);
      else{
        if(strpbrk(parameter+1+strlen(test), ValidParamSuffix)!=(parameter+1+strlen(test))) return(0);
        // We've now checked the prefix, test characters and suffix, success
        strcpy(value, (parameter+2+strlen(test)));
        return(1);
      }
    case PARAME:
      if(strnicmp(parameter, test, strlen(test))) return(0); // no match
      if(*(parameter+strlen(test))!='=') return(0); // no match
      strcpy(value, parameter+strlen(test)+1);
      return(1);
  }
  return(0); /* huh? */
}


void CapatilizeParam(int argc, char *argv[])
{
  register int n,pos;

  for(n=1; n<argc; n++){
    for(pos=0; pos<strlen(argv[n]); pos++)
    argv[n][pos]=toupper(argv[n][pos]);
  }
}


int CheckAlienParam(char *parameter, char *v1, ...)
{
  // This function may be used to ensure that a given parameter is using
  // one of a list of valid test codes specified above.
  // non zero on failure (not found), zero on success
  char *cmp=v1, *cmpto;
  int debug;
  va_list ap;
  int quit=0, found=0;

  va_start(ap, v1);
  if(strpbrk(parameter, ValidParamPrefix)!=parameter){
    quit=1;
  };

  while(!quit && !found){
   if((cmpto=strpbrk(parameter+1, ValidParamSuffix))==NULL ){
      if(!strcmp(parameter+1, cmp)) found=1; // match found
    }
    else{
      debug = (int) (cmpto-(parameter+1));
      if(!strncmp(parameter+1, cmp, debug)) found=1;
    }
    cmp = va_arg(ap, char *);
    #ifdef __NT__
    if(cmp==NULL) quit=1;
    #else
    if(!*cmp) quit=1;
    #endif
  }
  va_end(ap);
  return(!found);
}
