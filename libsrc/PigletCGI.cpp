/*
** Piglet Productions
**
** FileName       : PigletCGI.cpp
**
** Implements     : PigletCGI
**
** Description
**
** PigletCGI provides parameter loading and decoding for CGI scripts.
**
**
** Initial Coding : Colin Turner
**
** Date           : 22nd December 1998
**
**
** Copyright applies on this file, and distribution may be limited.
** Copyright 1998
*/

/*
** Revision 1.02 - 13th August 2000 - Colin Turner
**
** Increased allocation for ParsedParameter in GetVariable by one byte. It
** seems not to have been quite long enough, but oddly no ill effects were
** seen in other scripts...
**
** Revision 1.01 - 7th August 2000 - Colin Turner
**
** Reduced the amount requested from stdin on POST by one byte. I artifically
** increased MemRequired by one byte to ensure NULL termination on the buffer.
** This meant that some scripts on some servers went into an infinite loop
** waiting for the extra byte from the server.
**
** Revision 1.00
**
*/

#include <PigletCGI.h>

/*****************************************************************************/
/*                                                                           */
/*                    S E R V I C E   F U N C T I O N S                      */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*                     P U B L I C   F U N C T I O N S                       */
/*                                                                           */
/*****************************************************************************/


/*
**    PigletCGI
**
** Default constructor. See overloaded form below and Constructor for more
** details.
**
**/
PigletCGI::PigletCGI()
{
  Constructor(0, 0);
}


/*
**    PigletCGI
**
** Alternative constructor, see Constructor for more details.
**
**    Parameters
**
**    flags       Various behavioural flags, defined in CGI.H
**    maxMemory   The maximum memory in bytes that PigletCGI will allow
**                for the parameter space. 
**
**/
PigletCGI::PigletCGI(unsigned long flags, unsigned long maxMemory)
{
  Constructor(flags, maxMemory);
}


/*
**    ~PigletCGI
**
** Destructor. This function deallocated all allocated memory in the object.
**
**/
PigletCGI::~PigletCGI()
{
  if(Parameters){
    delete Parameters;
    Parameters = NULL;
  }
  if(ParsedParameter){
    delete ParsedParameter;
    ParsedParameter = NULL;
  }
}


/*
**    ClearError
**
** Clears the internal error indicator. Only useful if you are manually
** handling errors by use of GetError. Overriding SignalError in a virtual
** class is a better option.
**
**/
void PigletCGI::ClearError()
{
  Error = 0;
}


/*
**    GetError
**
** Returns the current value of the internal error indicator. See ClearError
** above for more comments.
**
**    Returns
**
**    See above
**
**/
int PigletCGI::GetError()
{
  return(Error);
}


/*
**    GetVariableLength
**
** Assuming a "normal" encoding of variables in the CGI parameters, PigletCGI
** can decode the variables for you. This function returns the number of bytes
** that a variable's data (NOT the name) occupies. Note that this is an
** upperbound, it is calculated before decoding, which can decrease the size.
**
**    Parameters
**
**    variable    The name of the variable you require
**
**    Returns
**
**    The length in bytes sufficient to hold the data (see above), if the
**    requested variable is found, if not it returns zero.
**
**/
unsigned long PigletCGI::GetVariableLength(char * variable)
{
  if(!Enabled) return(0);

  unsigned long loop;
  unsigned long Length = 0;
  int Found = 0;
  char * start = GetVariablePosition(variable);

  if(!start) return(0);

  // Move to the end of the variable, and past the equals sign
  start += strlen(variable) + 1;
 
  for(loop = (start - Parameters); !Found && (loop < MemRequired); loop++){
    if(*(Parameters + loop) == '&') Found = 1;
    else Length++;
  }

  if(!Found) Length--;
  return(Length);
}


/*
**    GetVariable
**
** This function locates and decodes a required variable which is encoded in
** a standard manner.
**
**    Parameters
**
**    variable    The name of the variable required
**
**    Returns
**
**    If the variable could not be found NULL is retured, otherwise
**    a pointer to a string storing the variable data. This string is decoded,
**    and NULL terminated. Memory is allocated for it automatically by the
**    the object, and destroyed on subsequent calls or object destruction.
**
**/
char * PigletCGI::GetVariable(char * variable)
{
  if(!Enabled) return(NULL);

  unsigned long VarLength;

  // If we're already using storage, close it down
  if(ParsedParameter){
    delete ParsedParameter;
    ParsedParameter = NULL;
  }

  VarLength = GetVariableLength(variable);
  if(!VarLength) return(NULL);
  else{
    ParsedParameter = new char[(const unsigned long) VarLength + 1];
    if(!ParsedParameter){
      // Memory allocation failure
      SignalError(5);
      return(NULL);
    }
  }

  GetVariable(variable, ParsedParameter);
  
  return(ParsedParameter);
}


/*
**    GetVariable
**
** Similar to the overloaded version above, except that the memory for any
** decoded variable is passed into the function.
**
**    Parameters
**
**    variable    The name of the variable required
**    buffer      A pointer to storage for the decoded data
**
**    Returns
**
**    buffer if the variable could be found and decoded, otherwise NULL
**
**/
char * PigletCGI::GetVariable(char * variable, char * buffer)
{
  if(!Enabled) return(NULL);

  int  Finished = 0;
  char HiXDigit;
  char LoXDigit;
  char HiXNumber;
  char LoXNumber;
  char * Input = GetVariablePosition(variable);
  char * Output = buffer;

  if(!Input) return(NULL);

  Input += strlen(variable) + 1;


  while(!Finished && ((Input - Parameters) < MemRequired)){
    switch(*Input){
      case '+' :
        *Output = ' ';
        Input++;
        Output++;
        break;
      case '%' :
        HiXNumber = 16;
        LoXNumber = 16;

        HiXDigit = *(++Input);
        LoXDigit = *(++Input);

        if(HiXDigit >= '0' && HiXDigit <= '9') HiXNumber = HiXDigit - '0';
        if(HiXDigit >= 'A' && HiXDigit <= 'F') HiXNumber = HiXDigit - 'A' + 10;
        if(HiXDigit >= 'a' && HiXDigit <= 'f') HiXNumber = HiXDigit - 'a' + 10;        

        if(LoXDigit >= '0' && LoXDigit <= '9') LoXNumber = LoXDigit - '0';
        if(LoXDigit >= 'A' && LoXDigit <= 'F') LoXNumber = LoXDigit - 'A' + 10;
        if(LoXDigit >= 'a' && LoXDigit <= 'f') LoXNumber = LoXDigit - 'a' + 10;

        if(HiXNumber == 16 || LoXNumber == 16){
          // Incorrect CGI encoding
          SignalError(6);
          return(NULL);
        }
        else *Output = (HiXNumber * 16) + (LoXNumber);
        
        Input++;
        Output++; 
        break;
      case '&' :
        Finished = 1;
        *Output = 0;
        break;
      default:
        *Output = *Input;
        Input++;
        Output++;
        break;
    }

  }

  return(buffer);
}

#if 0
/*
**    GetFirstVariable
**
** As an alternative for fetching specific variables, it is possible to go
** through the variable space decoding each variable in turn. This allows a
** script to analyse all variable data.
**
** See also GetNextVariable.
**
**    Parameters
**
**    variable    A buffer to insert the variable name into   
**    value       A buffer to insert the decoded variable data into
**
**    Returns
**
**    0 on success, 1 on failure (no variables to decode)
**    On failure make no assumptions as to the contents of variable and value
**
**/
int PigletCGI::GetFirstVariable(char * variable, char * value)
{
  if(!Enabled) return 1;

  // Initialise the CurrentParameter pointer
  CurrentParameter = Parameters;

  return(GetNextVariable(variable, buffer);
}


/*
**    GetNextVariable
**
** Continues a variable by variable search initiated by GetFirstVariable.
**
**    Parameters
**
**    variable    See GetFirstVariable
**    value       See GetFirstVariable
**
**    Returns
**
**    0 on success, 1 on failure (no more variables to decode)
**
**/
int PigletCGI::GetNextVariable(char * variable, char * value)
{
  int success = 1;

  if(!Enabled) return 1;

  // Bad pointer
  if(!CurrentParameter) return 1;

  // Off end of parameter space
  if(CurrentParameter > Parameters + MemRequired) return 1;

  success &= DecodeUntil(&CurrentParameter, variable, PigletCGI_Variable);
  if(success) success &= DecodeUntil(&CurrentParameter, value, PigletCGI_Value); 

  return(success);
}

#endif



/*
**    GetParameterSpace
**
** Provides a pointer to the internal copy of the CGI data, totally encoded.
** This function is of use only if you plan to implement your own parsing.
**
** See also GetParameterSpaceLength
**
**    Returns
**
**    A pointer to the parameter space if such exists (no failures);
**    NULL otherwise
**
**/
char * PigletCGI::GetParameterSpace()
{
  return(Parameters);
}


/*
**    GetParameterSpaceLength
**
** Returns the memory allocated to the parameter space in bytes. Note that
** this was the requested memory amount from the CGI environment. It is
** possible that no data was in fact loaded (check using GetParameterSpace).
**
**    Returns
**
**    The number of bytes requested by the CGI environment for data storage
**
**/
unsigned long PigletCGI::GetParameterSpaceLength()
{
  return(MemRequired);
}


/*
**    PrintDebugInfo
**
** As the name suggests, a function primarily for debugging this code. This
** function prints various parsed information to the web browser.
**
**/
void PigletCGI::PrintDebugInfo()
{
  printf("Content-type: text/html\n\n");

  printf("<HTML>\n");
  printf("<BODY>\n");

  printf("<H1>PigletCGI Module Debug Output</H1><P>\n");

  printf("<UL>\n");
  printf("<TABLE>\n");


  printf("<TR><TD></TD><TD></TD></TR>\n");
  printf("<TR><TD><B>CGI Variables</B>\n");
  printf("<TR><TD></TD><TD></TD></TR>\n");

  printf("<TR><TD>REQUEST_METHOD </TD><TD>%s</TD></TR>\n", EnvRequestMethod);
  printf("<TR><TD>QUERY_STRING </TD><TD>%s</TD></TR>\n", EnvQueryString);
  printf("<TR><TD>PATH_INFO </TD><TD>%s</TD></TR>\n", EnvPathInfo);
  printf("<TR><TD>CONTENT_LENGTH </TD><TD>%s</TD></TR>\n", EnvContentLength);
  printf("<TR><TD>CONTENT_TYPE </TD><TD>%s</TD></TR>\n", EnvContentType);

  printf("<TR><TD></TD><TD></TD></TR>\n");
  printf("<TR><TD><B>Internal Variables</B></TD><TD></TD></TR>\n");
  printf("<TR><TD></TD><TD></TD></TR>\n");

  printf("<TR><TD>Flags </TD><TD>%lX</TD></TR>\n", Flags);
  printf("<TR><TD>MaxMemory </TD><TD>%lu bytes</TD></TR>\n", MaxMemory);
  printf("<TR><TD>MemRequired </TD><TD>%lu bytes</TD></TR>\n", MemRequired);
  printf("<TR><TD>ParsedParameter </TD><TD>%s</TD></TR>\n", ParsedParameter);
  printf("<TR><TD>Error </TD><TD>%u</TD></TR>\n", Error);
  printf("<TR><TD>Encoded </TD><TD>%u</TD></TR>\n", Encoded);
  printf("<TR><TD>Enabled </TD><TD>%u</TD></TR>\n", Enabled);

  printf("</TABLE>\n");
  printf("</UL>\n");

  printf("</BODY>\n");
  printf("</HTML>\n");

}


/*****************************************************************************/
/*                                                                           */
/*             I M P L E M E N T A T I O N   F U N C T I O N S               */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*                    P R I V A T E   F U N C T I O N S                      */
/*                                                                           */
/*****************************************************************************/


/*
**    Constructor
**
** The private constructor function called by the public constructors. Performs
** simple initialization and calls LoadParameters. See that function for more
** details.
**
**    Parameters
**
**    flags       Flags for the constructed object, see CGI.H for more details
**    maxMemory   The maximum memory to allow for parameter space
**
**    Returns
**
**    Success of LoadParameters call
**
**/
int PigletCGI::Constructor(unsigned long flags, unsigned long maxMemory)
{
  memset(this, 0, sizeof(PigletCGI));
  EnvRequestMethod = getenv("REQUEST_METHOD");
  EnvQueryString   = getenv("QUERY_STRING");
  EnvPathInfo      = getenv("PATH_INFO");
  EnvContentLength = getenv("CONTENT_LENGTH");
  EnvContentType   = getenv("CONTENT_TYPE");
  ParsedParameter  = NULL;

  Enabled = 0;

  Flags = flags;
  MaxMemory = maxMemory;

  return(LoadParameters());
}


/*
**    LoadParameters
**
** This function determines whether the program is running in a CGI server
** environment, what request method is being used, whether the amount of
** requested memory is allowable and obtains the parameters from the correct
** source.
**
** If MaxMemory is set to zero, then only available memory (from new) will
** limit the parameter space that may be loaded. Any other value will cause
** the script to fail if more space is passed that allowed.
**
** If this function fails for whatever reason, no CGI services will be
** available; the object will set its internal state to disabled and will not
** respond to any information requests.
**
**    Returns
**
**    1 on success, 0 on failure
**
**/
int PigletCGI::LoadParameters()
{
  int Type = 0;
  MemRequired = 0;

  // Check for valid request method
  if(!EnvRequestMethod){
    // Doesn't look like we're in a CGI server environment
    SignalError(1);
    return(0);
  }
  else{
    if(!stricmp(EnvRequestMethod, "GET")) Type = 1;
    if(!stricmp(EnvRequestMethod, "POST")) Type = 2;
  }

  switch(Type){
    case 1 : // Get
      if(Flags & PigletCGI_NoGet){
        // Disallowed request method
        SignalError(3);
        return(0);
      }
      break;
    case 2 : // Post
      if(Flags & PigletCGI_NoPost){
        // Disallowed request method
        SignalError(3);
        return(0);
      }
      break;
    default:
      // Invalid Request Method
      SignalError(2);
      return(0);
  }

  // Now allocate space for encoded data
  switch(Type){
    case 1 : // Get
      MemRequired = strlen(EnvQueryString) + 1;
      break;
    case 2 : // Post
      if(!EnvContentLength) return(1);
      MemRequired = (unsigned long) atol(EnvContentLength) + 1;
      break;
  }

  if(!MemRequired) return(1);
  
  if(MaxMemory && (MemRequired > MaxMemory)){
    // More memory required than permitted
    SignalError(4);
    return(0);
  }

  Parameters = new char[(const unsigned long) MemRequired];
  if(!Parameters){
    // Memory allocation failure
    SignalError(5);
    return(0);
  }


  // Load Data
  switch(Type){
    case 1 : // Get
      Encoded = 1;
      memcpy(Parameters, EnvQueryString, MemRequired);
      break;
    case 2 : // Post
      if(!stricmp(EnvContentType, "application/x-www-form-urlencoded")) Encoded = 1;
      fread(Parameters, 1, MemRequired-1, stdin);
      break;
  }
  *(Parameters + MemRequired - 1) = 0;

  Enabled = 1;
  return(1);
}


/*
**    SignalError
**
** This function is used to handle error conditions. In this base class it
** simply sets the internal error indicator to the appropriate number. See
** CGI.H for details.
** This function is called by the class when it encounters a problem.
**
**    Parameters
**
**    newError    The new error code to assign
**
**/
void PigletCGI::SignalError(int newError)
{
  Error = newError;
}


/*
**    GetVariablePosition
**
** This function finds the start of a data portion for a given variable (which
** is encoded in a standard way)
**
**    Parameters
**
**    variable    The name of the variable you require
**
**    Returns
**
**    If the variable can be found, this returns a pointer to the start of the
**    variable data (which is simply in the parameter space and is NOT
**    terminated in generality).
**    If the variable cannot be found, NULL is returned.
**
**/
char * PigletCGI::GetVariablePosition(char * variable)
{
  if(!Enabled) return(NULL);

  int Found = 0;
  int Potential;
  unsigned long loop;

  for(loop = 0; loop < MemRequired && !Found; loop++){
    // Is there room for it be present (remember the equals sign)?
    if((MemRequired - loop) < (strlen(variable) + 1)) break;
    Potential=!memcmp(Parameters + loop, variable, strlen(variable));

    if(Potential && !loop){
      if(*(Parameters + loop + strlen(variable)) == '=') Found = 1;
    }

    if(Potential && loop){
      if((*(Parameters + loop - 1) == '&') &&
          *(Parameters + loop + strlen(variable)) == '=') Found = 1;
    }
  }

  if(Found) return(Parameters + loop - 1);
  return(NULL);
}

#if 0

/*
**    DecodeUntil
**
** This function will decode standard CGI encoded data, from inside the
** parameter space. It will cease decoding as soon as it meets a specified
** character. This character will not be placed in the decoded data.
** The from pointer will be placed on the character following the termination
** character. The data will be zero terminated.
**
**    Parameters
**
**    from      A pointer to a pointer in memory to decode from
**    to        A buffer into which decoded material is inserted
**    until     The character at which decoding stops
**
**    Returns
**
**    1 on success, 0 on failure (data unreliable)
**
**/
int PigletCGI::DecodeUntil(char ** from, char * to, char until)
{
  int Finished = 0;
  char HiXDigit;
  char LoXDigit;
  char HiXNumber;
  char LoXNumber;

  if(!Enabled) return(0);

  // Invalid pointer
  if(!*from) return(0);
  if((*from < Parameters) || (*from > Parameters + MemRequired)) return(0);

  while(!Finished && ((*from - Parameters) < MemRequired)){
    switch(**from){
      case '+' :
        *to = ' ';
        *from++;
        to++;
        break;
      case '%' :
        HiXNumber = 16;
        LoXNumber = 16;
  
        HiXDigit = **(++(*from));
        LoXDigit = **(++(*from));
  
        if(HiXDigit >= '0' && HiXDigit <= '9') HiXNumber = HiXDigit - '0';
        if(HiXDigit >= 'A' && HiXDigit <= 'F') HiXNumber = HiXDigit - 'A' + 10;
        if(HiXDigit >= 'a' && HiXDigit <= 'f') HiXNumber = HiXDigit - 'a' + 10;        
  
        if(LoXDigit >= '0' && LoXDigit <= '9') LoXNumber = LoXDigit - '0';
        if(LoXDigit >= 'A' && LoXDigit <= 'F') LoXNumber = LoXDigit - 'A' + 10;
        if(LoXDigit >= 'a' && LoXDigit <= 'f') LoXNumber = LoXDigit - 'a' + 10;
  
        if(HiXNumber == 16 || LoXNumber == 16){
          // Incorrect CGI encoding
          SignalError(6);
          return(NULL);
        }
        else *to = (HiXNumber * 16) + (LoXNumber);
  
        *from++;
        to++; 
        break;
      case until :
        Finished = 1;
        *to = 0;
        *from++;
        break;
      default:
        *to = **from;
        *from++;
        to++;
        break;
    }
  }
  
  return(Finished);
}

#endif
