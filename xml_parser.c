
/**************************************************************************************
Parser
**************************************************************************************/
int parse(char* xml,void (*eventHandler)(char*, char*))
{
  customEventHandler = eventHandler;
  if(parse_document(&xml))
  {
    return 1;
  }
  return 0;
}
/**************************************************************************************
document           ::=           prolog  element  Misc*
**************************************************************************************/
int parse_document(char** xml)
{
  raiseEvent("startDocument","");
  if(parse_prolog(xml))    return 1;
  if(parse_element(xml))   return 1;
  parse_misc(xml);
  raiseEvent("endDocument","");
  return 0;
}

/**************************************************************************************
prolog     ::=           XMLDecl? Misc* (doctypedecl  Misc*)?
**************************************************************************************/
int parse_prolog(char** xml)
{
  if(parse_XMLDecl(xml))      return 1;
  parse_misc(xml);
  if(parse_doctypedecl(xml))  return 1;
  parse_misc(xml);
  return 0;
}
/**************************************************************************************
        XMLDecl    ::=          '<?xml' VersionInfo  EncodingDecl? SDDecl? S? '?>'
**************************************************************************************/
int parse_XMLDecl(char** xml)
{
  if(parse_literal(xml,'<')!=0)    return 1;
  if(parse_literal(xml,'?')!=0)    return 1;
  if(parse_literal(xml,'x')!=0)    return 1;
  if(parse_literal(xml,'m')!=0)    return 1;
  if(parse_literal(xml,'l')!=0)    return 1;
  if(parse_S(xml))         return 1;
  while (getCharacter(xml)!='?') {};
  if(parse_literal(xml,'>')==1)       return 1;
  return 0;
}
/**************************************************************************************
doctypedecl        ::=          '<!DOCTYPE' S  Name (S  ExternalID)? S? ('[' intSubset ']' S?)? '>'
**************************************************************************************/
int parse_doctypedecl(char** xml)
{
  char szName[ELEMENT_NAME_MAX_SIZE];
  if(parse_literal(xml,'<')==1) return 1;
  if(parse_literal(xml,'!')==1) return 1;
  if(parse_literal(xml,'D')==1) return 1;
  if(parse_literal(xml,'O')==1) return 1;
  if(parse_literal(xml,'C')==1) return 1;
  if(parse_literal(xml,'T')==1) return 1;
  if(parse_literal(xml,'Y')==1) return 1;
  if(parse_literal(xml,'P')==1) return 1;
  if(parse_literal(xml,'E')==1) return 1;
  if(parse_S(xml))         return 1;
  if(parse_Name(xml,szName)) return 1;
  parse_S(xml);
  while (getCharacter(xml)!='>')  {};
  raiseEvent("doctypedecl",szName);
  return 0;
}
/**************************************************************************************
        element    ::=           EmptyElemTag | STag content ETag
**************************************************************************************/
int parse_element(char** xml)
{
  char szSTagName[ELEMENT_NAME_MAX_SIZE];
  char szETagName[ELEMENT_NAME_MAX_SIZE];
  parse_S(xml);
  if(!parse_EmptyElemTag(xml))
  {
    return 0;
  }
  else
  {
    if(!parse_STag(xml,szSTagName))
    {
      if(!parse_Content(xml))
      {
        if(!parse_ETag(xml,szETagName))
        {
          if(strcmp(szSTagName,szETagName)!=0) 
            {
              return 1;
            }
          return 0;
        }
      else
        {
          return 1;
        }
      }
      else
      {
        return 1;
      }
    }
    else
    {
      return 1;
    }
  }
}
/**************************************************************************************
STag       ::=          '<' Name (S  Attribute)* S? '>'
**************************************************************************************/
int parse_STag(char** xml,char *szSTagName)
{
  char c;
  char szName[ELEMENT_NAME_MAX_SIZE]="";
  char* pos1;
  /*strini(szName,ELEMENT_NAME_MAX_SIZE);*/
  pos1=*xml;
  c = getCharacter(xml);
  if(c!='<')
  {
    *xml=pos1;
    return 1;
  }
  if(parse_Name(xml,szName))
  {
    *xml=pos1;
    return 1;
  }
  c = getCharacter(xml);
  if(c!='>') {
    return 1;
  }
  strcpy(szSTagName,szName);
  raiseEvent("startElement",szName);
  return 0;
}
/**************************************************************************************
ETag       ::=          '</' Name  S? '>'
**************************************************************************************/
int parse_ETag(char** xml,char *szETagName)
{
    char c;
    char szName[ELEMENT_NAME_MAX_SIZE]="";
    char szNameTemp[ELEMENT_NAME_MAX_SIZE];
    char* pos1;
    pos1=*xml;
    /*strini(szName,ELEMENT_NAME_MAX_SIZE);*/
    c = getCharacter(xml);
    if(c!='<')
    {
      *xml=pos1;
      return 1;
    }
    c = getCharacter(xml);
    if(c!='/')
    {
      *xml=pos1;
      return 1;
    }
    if(parse_Name(xml,szName))
    {
      *xml=pos1;
      return 1;
    }
    c = getCharacter(xml);
    if(c!='>')
    {
      *xml=pos1;
      return 1;
    }
    strcpy(szETagName,szName);
    raiseEvent("endElement",szName);    
    return 0;
}
/**************************************************************************************
        EmptyElemTag       ::=          '<' Name (S  Attribute)* S? '/>'
**************************************************************************************/
int parse_EmptyElemTag(char** xml)
{
    char c;
    char szName[ELEMENT_NAME_MAX_SIZE]="";
    char* pos1;
    pos1 = *xml;
    /*strini(szName,ELEMENT_NAME_MAX_SIZE);*/
    if(parse_literal(xml,'<')==1)
    {
      *xml=pos1;
      return 1;
    }
    if(parse_Name(xml,szName))
    {
      *xml=pos1;
      return 1;
    }
    if(parse_literal(xml,'/')==1)
    {
      *xml=pos1;
      return 1;
    }
    if(parse_literal(xml,'/')==1) return 1;
    raiseEvent("EmptyElemTag",szName);
    return 0;
}
/**************************************************************************************
content    ::=           CharData? ((element | Reference | CDSect | PI | Comment) CharData?)*
**************************************************************************************/
int parse_Content(char** xml)
{
  char c;
  char* pos1;
  char szText[CONTENT_TEXT_MAX_SIZE]="";
  int i;

  /*strini(szText,CONTENT_TEXT_MAX_SIZE);*/
  
  i=0;
  pos1 = *xml;
  parse_S(xml);
  c = getCharacter(xml);
  if(c=='<')
  {
    (*xml)--;
    while(!parse_element(xml))
    {

    }
    return 0;
  }
  else
  {
    *xml = pos1;
    c=getCharacter(xml);
    szText[i] = c;
    i++;
    while(c!='<' && c!='>')
    {
      c = getCharacter(xml);
      szText[i] = c;
      i++;
    }
    i--;
    szText[i] = '\0';
    (*xml)--;
    if (pos1==*xml)
    {
      return 1;
    }
    raiseEvent("characters", szText);
    return 0;
  }
}
/**************************************************************************************
Name       ::=          (Letter | '_' | ':') (NameChar)*
**************************************************************************************/
int parse_Name(char** xml,char* szName)
{
  char* pos1;
  char c;
  char szTemp[ELEMENT_NAME_MAX_SIZE];
  strini(szTemp,ELEMENT_NAME_MAX_SIZE);
  pos1 = *xml;
  if (parse_Letter(xml,&c))
  {
    c=getCharacter(xml);
    if( (c!='_') && (c!=':') )
    {
      *xml = pos1;
      return 1;
    }
  }
  szName[0] = c;
  szName[1] = '\0';
  if (parse_NameChar(xml,szTemp))
  {
    *xml = pos1;
    return 1;
  }
  strcat(szName,szTemp);
  return 0;
}
/**************************************************************************************
Letter     ::=           BaseChar | Ideographic
**************************************************************************************/
int parse_Letter(char** xml,char* letter)
{
  char* pos1;
  char c='\0';
  pos1 = *xml;
  c = getCharacter(xml);
  *letter=c;
  if ( !( (c>='A' && c<='Z') || (c>='a' && c<='z') ) )
  {
    *xml = pos1;
    return 1;
  }
  return 0;
}
/**************************************************************************************
NameChar           ::=           Letter | Digit | '.' | '-' | '_' | ':' | CombiningChar | Extender
**************************************************************************************/
int parse_NameChar(char** xml,char* szName)
{
  char c='\0';
  int i=0;
  c = getCharacter(xml);
  szName[0]=c;
  i++;
  while(
          (c>='1' && c<='0') || (c>='A' && c<='Z') || (c>='a' && c<='z') ||
          (c=='.') || (c=='_') || (c==':') || (c=='-')
       )
  {
    c = getCharacter(xml);
    szName[i] = c;
    i++;
  }
  i--;
  szName[i] = '\0';
  (*xml)--;
  return 0;
}
/**************************************************************************************
Misc       ::=           Comment | PI | S
**************************************************************************************/
int parse_misc(char** xml)
{
  if(parse_S(xml)) return 1;
  /*raiseEvent("Misc","");*/
  return 0;
}
/**************************************************************************************

**************************************************************************************/
int parse_literal(char** xml,char cLiteral)
{
  char c;
  char* pos1;
  pos1 = *xml;
  c = getCharacter(xml);
  if(c!=cLiteral)
  {
    *xml = pos1;
    return 1;
  }
  else
  {
    return 0;
  }
}
/**************************************************************************************

**************************************************************************************/
char getCharacter(char** xml)
{
  char c='\0';
  c = **xml;
  (*xml)++;
  return c;
}
/**************************************************************************************
S          ::=          (#x20 | #x9 | #xD | #xA)+
**************************************************************************************/
int parse_S(char** xml)
{
  if( !(((**xml)==' ') || ((**xml)==0x09) || ((**xml)==0x0A) || ((**xml)==0x0D)) )
  {
    return 1;
  }
  (*xml)++;
  while( ((**xml)==' ') || ((**xml)==0x09) || ((**xml)==0x0A) || ((**xml)==0x0D) )
  {
    (*xml)++;
  }
  return 0;
}
/**************************************************************************************

**************************************************************************************/
void raiseEvent(char* szEventName, char* szText)
{
  customEventHandler(szEventName,szText);
}
/**************************************************************************************

**************************************************************************************/
void strini(char* s,long l)
{
  long i=0;
  for(i=0;i<l;i++) s[i] = '\0';
}

