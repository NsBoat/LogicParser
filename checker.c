#include <stdio.h> 
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */


const int Fsize=50;
int no_edges;
int no_nodes;
int i;
const int cases=10;

int leftBracket = 0;
int rightBracket = 0;
int binaryConnective = 0;
int legalUsed = 0;
int result;

char valid = 'T'; 

int parse(char *g)
{/* return 1 if atomic, 2 if  neg, 3 if binary, 4 if exists, 5 if for all, ow 0*/

  if (*g != '\0'){
    switch (*g){
      case 'X':
        if (*(g+1) == '[' && (*(g+2) == 'x' || *(g+2) == 'y' || *(g+2) == 'z') && (*(g+3) == 'x' || *(g+3) == 'y' || *(g+3) == 'z') && *(g+4) == ']'){
          valid = 'T';
          parse(g+5);
        }
        else{
          valid = 'F';
        }
        break;
      case '-':
        if (*(g+1) == '\0'){
          valid = 'F';
        }
        else{
          valid = 'T';
          parse(g+1); 
        }
        break;
      case 'E':
      case 'A':
        if (*(g+1) == 'x' || *(g+1) == 'y' || *(g+1) == 'z'){
          if (*(g+2) == '\0'){
            valid = 'F';
          }
          else{
            valid = 'T';
            parse(g+2);
          }
        }
        else{
          valid = 'F';
        } 
        break;
        case '(':
        if (*(g+1) == ')' || *(g+1) == '^' || *(g+1) == 'v' || *(g+1) == '>'){
          valid = 'F';
        }
        else{
          valid = 'T';
          leftBracket += 1;
          parse(g+1);
        }
        break;
      case '^':
      case 'v':
      case '>':
        if (*(g+1) == ')' || g == 0){
          valid = 'F';
        }
        else{
          valid = 'T';
          binaryConnective += 1;
          legalUsed += 1;
          if (legalUsed > 1){
            valid = 'F';
          }
          else{
            parse(g+1);
          }
        }
        break; 
      case ')':
        if ((*(g+1) != '\0' && *(g+1) != '^' && *(g+1) != 'v' && *(g+1) != '>') || g == 0){
          valid = 'F';
        }
        else{
          valid = 'T';
          rightBracket += 1;
          legalUsed -= 1;
          if(legalUsed < 0){
            valid = 'F';
          }
          else{
            parse(g+1);
          } 
          break; 
        }
      default:
        valid = 'F';                  
    }
  }

  if ((leftBracket != rightBracket) || (leftBracket != binaryConnective)){
    valid = 'F';
  } 
  
  leftBracket = 0;
  rightBracket = 0;
  binaryConnective = 0;
  legalUsed = 0;

  if (valid == 'T'){
    switch (*g){
        case 'X':return(1);
        case '-':return(2);
        case '(':return(3);
        case 'E':return(4);
        case 'A':return(5);
        default:return(0);             
      }
    }
    else{
      return(0);
    }
}



int eval(char *nm, int edges[no_edges][2], int size, int V[3])
{/* returns 1 if formla nm evaluates to true in graph with 'size' nodes, no_edges edges, edges stored in 'edges', variable assignment V.  Otherwise returns 0.*/

int q, p, i;
  int Vnew[3];
  int bool = 0;
  int scope = 0;
  int flag = 0;

  for (i = 0; i < 3; i++){
    Vnew[i] = V[i];
  }
  
  switch (*nm){
    case 'X':
      nm += 2;
      switch (*nm){
        case 'x' : q = 0; break;
        case 'y' : q = 1; break;
        case 'z' : q = 2; break;
      }
      nm++;
      switch (*nm){
        case 'x' : p = 0; break;
        case 'y' : p = 1; break;
        case 'z' : p = 2; break;
      }
    for (i = 0; i < no_edges; i++){
      if (edges[i][0] == V[q] && edges[i][1] == V[p]){
        bool = 1;
      }
    }
    return (bool);
    case '-':
      if (eval(nm+1, edges, size, V) == 1) return 0;
      else return 1;
      break; 

    case 'E':
      nm++;
      switch (*nm){
        case 'x':
          for (i = 0; i < size; i++){
            Vnew[0] = i;
            if (eval(nm+1, edges, size, Vnew) == 1){
              return (1);
              break;
            }
          }
          return(0);
          break;
        case 'y':
          for (i = 0; i < size; i++){
            Vnew[1] = i;
            if (eval(nm+1, edges, size, Vnew) == 1){
              return (1);
              break;
            }
          }
          return(0);
          break;
        case 'z':
          for (i = 0; i < size; i++){
            Vnew[2] = i;
            if (eval(nm+1, edges, size, Vnew) == 1){
              return (1);
              break;
            }
          }
          return(0);
          break;
      }
      break;  
    case 'A':
      nm++;
      switch (*nm){
        case 'x':
          for (i = 0; i < size; i++){
            Vnew[0] = i;
            if (eval(nm+1, edges, size, Vnew) == 0){
              return (0);
            }
          }
            return(1);
          break;
        case 'y':
          for (i = 0; i < size; i++){
            Vnew[1] = i;
            if (eval(nm+1, edges, size, Vnew) == 0){
              return (0);
            }
          }
          return(1);
          break;
        case 'z':
          for (i = 0; i < size; i++){
            Vnew[2] = i;
            if (eval(nm+1, edges, size, Vnew) == 0){
              return (0);
            }
          }
                    return(1);          
          break;
      }
      break;
    case '(':
      p = eval(nm+1, edges, size, V);
      while (*nm != '\0'){
        switch (*nm){
          case '(' : scope++; break;
          case ')' : scope--; break;
          case '^' :
          case 'v' :
          case '>' :
            if (scope == 1) {
              flag = 1;
              break;
            } 
        }
        if (flag == 1) break;
        nm++;
      }   
      q = eval(nm+1, edges, size, V);
      switch (*nm){
        case '^':
          if (p == 1 && q == 1) return (1);
          else return (0);
          break;
        case 'v':
          if (p == 1 || q == 1) return (1);
          else return (0);
          break;
        case '>':
          if (p == 1 && q == 0) return (0);
          else return (1);  
          break;  
      }
  }
  return (0);
}



int main()
{
  char *name=malloc(Fsize); /*create space for the formula*/
  FILE *fp, *fpout;
 
  /* reads from input.txt, writes to output.txt*/
 if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
  if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}

  int j;
  for(j=0;j<cases;j++)
    {
      fscanf(fp, "%s %d %d",name,&no_nodes,&no_edges);/*read number of nodes, number of edges*/
      int edges[no_edges][2];  /*Store edges in 2D array*/
      for(i=0;i<no_edges;i++)	 fscanf(fp, "%d%d", &edges[i][0], &edges[i][1]);/*read all the edges*/

      /*Assign variables x, y, z to nodes */
      int W[3];
      /*Store variable values in array
	value of x in V[0]
	value of y in V[1]
	value of z in V[2] */
      for(i=0;i<3;i++)  fscanf(fp, "%d", &W[i]);
      int p=parse(name); 
      switch(p)
	{case 0:fprintf(fpout,"%s is not a formula.  ", name);break;
	case 1: fprintf(fpout,"%s is an atomic formula.  ",name);break;
	case 2: fprintf(fpout,"%s is a negated formula.  ",name);break;
	case 3: fprintf(fpout,"%s is a binary connective formula.  ", name);break;
	case 4: fprintf(fpout,"%s is an existential formula.  ",name);break;
	case 5: fprintf(fpout,"%s is a universal formula.  ",name);break;
	default: fprintf(fpout,"%s is not a formula.  ",name);break;
	}
  
      /*Now check if formula is true in the graph with given variable assignment. */
      if (parse(name)!=0){
	if (eval(name, edges, no_nodes,  W)==1)	fprintf(fpout,"The formula %s is true in G under W\n", name);
	else fprintf(fpout,"The formula %s is false in G under W\n", name);
      }
    }


 
  fclose(fp);
  fclose(fpout);
  free(name);
  return(0);
}