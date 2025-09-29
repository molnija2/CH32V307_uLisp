#include <stdio.h>
#include <inttypes.h>
#include <malloc.h>
#include <string.h>
#include <dirent.h>

typedef struct{
	uint8_t Height ;
	uint8_t Width ;
	uint8_t Style ;
	uint8_t Dir ;
	char name[64] ;
    uint32_t DataPtr ;
} FONT_INFO;

FONT_INFO	Font;

FILE *File ;

uint16_t *FontData ;

char outfilename[256] ;

int TransformFont(char *filename);



int fillpattern(char *mask, char *pattern)
{
    int i = 0 ;
    if(*mask==0) return -1 ;
    while((*mask!=0)&&(*mask!='*'))
    {
        *pattern++ = *mask++ ;
        i++ ;
    }
    *pattern = 0 ;

    return i ;
}

int findpattern(char *pattern, char *name)
{
    int i = 0 , lenp, lenn ;
    if(*pattern==0) return -1 ;
    lenp = strlen(pattern) ;
    lenn = strlen(name) ;
    while(lenp<=lenn)
    {
        if(strncmp(name,pattern,lenp)==0) return i;
        //if(strcmp(name,pattern)==0) return i;
        name++ ;
        lenn-- ;
        i++ ;
    }
    return -1 ;
}



int selection(char *name, char *filemask )
{
    char file_pattern[256] ;
    int i;
    int imaskpos = 0, inamepos = 0 ;
    int find ;
    i = fillpattern(filemask, file_pattern);
    if(i==-1) return -1 ;

    if(i>0)
    {
        if(strncmp(file_pattern,name,i)!=0)
            return 0 ;
    }

    imaskpos += i ; // next position after '*'

    if(filemask[imaskpos] == '*') {
        find = 1 ;  // search continue
        imaskpos++ ;
    }
    else find = 0 ;

    inamepos += i ;
    do{
        // take mask next fragment between '*' symbols
        i = fillpattern(&filemask[imaskpos], file_pattern);

        if(i == -1 ) {
            if(name[inamepos]==0x0) return 1 ;
            if(find) return 1 ;   // because mask last symbol is '*'
        }

        int k = findpattern(file_pattern, &name[inamepos]);
        if(k==-1) return 0 ;

        imaskpos += i ;
        inamepos += k ;
        if(filemask[imaskpos] == '*') {
            find = 1 ;  // search continue
            imaskpos++ ;
        }
        else
            if(name[inamepos+i] != 0) return 0 ;
            // the end of pattern but not end of name
    }while(1) ;

return 0;
}

void main(int iArgs, char **Aargs)
{
    int n = 0;
    if(iArgs < 2)
    {
        printf("No arguments a found\nSearch all font files:\n") ;
        
        char dirname_string[256] ;
        char pattern_string[256] = "*.h" ;
        int type = 0x4 | 0x8 ;
        
        getcwd(dirname_string, 256);
        
        DIR *Dir;
        Dir=opendir(dirname_string);
        while (1) {

            struct dirent *Dirent = readdir(Dir);
            if(!Dirent) break;
            
            //printf("Found file %s\n", Dirent->d_name ) ;

            if((Dirent->d_type & type)
                &&(selection((char*)Dirent->d_name, pattern_string )))
            {
                n+=TransformFont(Dirent->d_name) ;
            }

  };

  closedir(Dir);

        
        
    }
    else
        n+=TransformFont(Aargs[1]) ;
    
    //printf("Input file '%s'\n" , Aargs[1]) ;
    
    
    printf("%d files were transformed\n", n) ;
    
    
}



int TransformFont(char *filename)
{
    File = fopen(  filename, "rb" );
    if(!File) return(0);
    
    strcpy(outfilename, filename) ;
    
    uint8_t data ;

    do fread( &data, 1, 1, File);
    while((data != '(')&&(feof(File)==0)) ;

    fscanf(File,"%d,%d, %d, %d,\n", &Font.Height, &Font.Width, &Font.Style, &Font.Dir) ;
      
      
    do fread( &data, 1, 1, File);
    while((data != '"')&&(feof(File)==0)) ;
    
	  if(feof(File)!=0)
      {
          printf("Error FontName scanning  '%s'\n", filename) ;
          fclose(File);
          return(0) ;
      }

    char *fontname = Font.name;
	  
	  do{
		  fread( &data, 1, 1, File);
		  *fontname++ = data ;
	  }
	  while((data != '"')&&(feof(File)==0)) ;
	  
	  if(feof(File)!=0)
      {
          printf("Error FontName end scanning\n") ;
          fclose(File);
          return(0) ;
      }
	  fontname-- ;
	  *fontname = 0 ;
	  
      printf("Font H=%d, W=%d, Style=%d, Dir=%d, Name='%s'  ", Font.Height, Font.Width, Font.Style, Font.Dir, Font.name) ;
	  
      
      FontData = malloc(256*Font.Height*sizeof(uint16_t)) ;
      
    do fread( &data, 1, 1, File);
    while((data != '{')&&(feof(File)==0)) ;
    
    if(feof(File)!=0)
    {
          printf("Error FontName '{' scanning\n") ;
          fclose(File);
          return(0) ;
    }

        do fread( &data, 1, 1, File);
        while((data != 'x')&&(feof(File)==0)) ;
        
    int i ;
    i=0;
    while((i<(256*Font.Height))&&(feof(File)==0))
    {
        uint8_t data ;
        uint32_t data32 ;
        

        if(fscanf(File,"%X", &data32)>0)
        {
            FontData[i] = data32 ;
            //printf("0x%04X ", Font.Data[i]) ;
            i++;
        }
        
        do fread( &data, 1, 1, File);
        while((data != 'x')&&(feof(File)==0)) ;

    }
      
	fclose(File);
      
    //printf("%d values  %d chars  readed\n\n", i, i/19) ;
      
    /*i = strlen(outfilename) ;
    while((i>0)&&(outfilename[i]!='.')) i-- ;
      
    if(i>0) outfilename[i] = 0 ;
    
    strcat(outfilename, ".bin") ;*/
    strncpy(outfilename, &filename[5],3) ;
    
    char str_style[4][3] = {"N","O","B","D"} ;
    char str_dir[2][2] = {"h","v"};
    
    sprintf(&outfilename[2],"%s%d%s%d.bin",str_style[Font.Style],Font.Height,str_dir[Font.Dir],Font.Width) ;
    
    printf("Output file: '%s'\n\n", outfilename ) ;
    
    File = fopen(outfilename,"wb") ;
    
    if(File)
    {
        fwrite(&Font, 1, sizeof(Font), File) ;
        fwrite(FontData, 256*Font.Height, sizeof(uint16_t), File) ;
        fclose(File) ;
    }
    else
    {
        printf("Cannot open the file for write\n\n") ;
        free(FontData);
        return 0 ;
    }
    
    free(FontData);
    
    return 1 ;
}
