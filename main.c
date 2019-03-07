#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 100

struct CHARS* expand(struct CHARS*,int);		// PROTOTYPE OF THE FUNCTION USED TO SEEK THE LINE

struct COMMAND{									
	char c1[MAX];								//	STRUCTURE USED TO KEEP COMMANDS
}*COM;											

struct CHARS                                    
{			                                	
	char type[MAX],name[MAX];					//	STRUCTURE USED TO KEEP CHARACTERS
	int hp,damage,x,y,xp,ox,oy;					
}*A;											

int main(int argc,char *argv[])
{
	FILE * ifp=fopen(argv[1],"r");				// We open the necessary files
	FILE * i2fp=fopen(argv[2],"r");
	FILE * ofp=fopen(argv[3],"w");
	
	if(ifp==NULL || i2fp==NULL)
	{
		printf("Error opening file");
	}
	
	int i=0,j=0,k=0,l=0,m=0,n=0,a,b,d=0,c,r,dead_MONSTER=0,dead_HERO=0,size=10,nMONSTER=0,nHERO=0,count=0,flag=1;
	
	char *B;

	A=(struct CHARS*)malloc(size*sizeof(struct CHARS)) ; //Dynamically allocated memory for characters

	for(;fscanf(ifp,"%s",A[i].type)!=EOF;i++,n++)
	{
		A=expand(A,size*2);
		
		B=strtok(A[i].type,",");		
		
		if(strcmp(A[i].type,"MONSTER")==0)				//sending properties of heros to pointers of structure

		{
			B=strtok(NULL,",");
			strcpy(A[i].name,B);
			B=strtok(NULL,",");
			sscanf(B,"%d",&A[i].hp);
			B=strtok(NULL,"\n");
			sscanf(B,"%d",&A[i].damage);
			nMONSTER++;
				
		}
		
		if(strcmp(A[i].type,"HERO")==0)					//sending properties of monsters to pointers of structure
		{
			B=strtok(NULL,",");
			strcpy(A[i].name,B);
			B=strtok(NULL,",");
			sscanf(B,"%d",&A[i].hp);
			B=strtok(NULL,"\n");
			sscanf(B,"%d",&A[i].damage);
			nHERO++;	
		}	
		
	}
	
	COM=(struct COMMAND*)malloc(MAX*10*sizeof(struct COMMAND));   //Dynamically allocated memory for commands
	
	for(i=0,k=0;fscanf(i2fp,"%s",COM[i].c1)!=EOF;i++,k++);
	
	i=0;
	
	r=atoi(COM[i+1].c1);
	
	c=atoi(COM[i+2].c1);
	
	char* map[r];
	
	for (a=0; a<r; a++)
        {
        map[a] = (char*)malloc(c*sizeof(char));					//Dynamically allocated memory for maps field
	}
	
	for(i=0;i<k;i++)
	{
		if(strcmp(COM[i].c1,"PUT")==0 && strcmp(COM[i+1].c1,"HERO")==0 )      //put command is performed for heroes
		{
			for(l=0;l<nHERO;l++)
			{
				for(m=0,d=3;m<nHERO;m++,d+=3)
				{
					if(strcmp(COM[i+d-1].c1,A[l].name)==0)
					{
						A[l].x=atoi(COM[i+d].c1);
						A[l].y=atoi(COM[i+d+1].c1);
					}
				}
			}	
			
			for (a=0;a<r;a++)
         	{
				for(b=0;b<c;b++)
				{
					map[a][b]='.';
				}
			}
			
			for(l=0;l<nHERO;l++)
			{
				map[A[l].x][A[l].y]=A[l].name[0];
			}
		}
		if(strcmp(COM[i].c1,"PUT")==0 && strcmp(COM[i+1].c1,"MONSTER")==0)		 //put command is performed for monsters
		{
			l=0;
			
			for(l+=nHERO;l<nHERO+nMONSTER;l++)
			{
				for(m=0,d=3;m<nHERO+nMONSTER;m++,d+=3)
				{
					if(strcmp(COM[i+d-1].c1,A[l].name)==0)
					{
						A[l].x=atoi(COM[i+d].c1);
						A[l].y=atoi(COM[i+d+1].c1);
					}
				}
			}
			
			l=0;
			
			for(l+=nHERO;l<nMONSTER+nHERO;l++)
			{
				map[A[l].x][A[l].y]=A[l].name[0];
			}
		}
		if(strcmp(COM[i].c1,"MOVE")==0 && strcmp(COM[i+1].c1,"HERO")==0)       //move command is performed
		{
			fprintf(ofp,"HEROES MOVED\n");
			
			for(l=0;l<nHERO+nMONSTER;l++)
			{
				for(m=0,d=3;m<nHERO+nMONSTER;m++,d+=3)
				{
					if(strcmp(COM[i+d-1].c1,A[l].name)==0)
					{
						A[l].ox=A[l].x;
						A[l].oy=A[l].y;
						A[l].x=atoi(COM[i+d].c1);
						A[l].y=atoi(COM[i+d+1].c1);
						
						if(A[l].x>=r || A[l].y>=c)
						{
							fprintf(ofp,"%s cant move.There is a wall.\n",A[l].name);
							A[l].x=A[l].ox;
							A[l].y=A[l].oy;
							break;
						}
						if(map[A[l].x][A[l].y]!='.')
						{
							fprintf(ofp,"%s can't move. Place is occupied.\n",A[l].name);
						}
						else 
						{
							map[A[l].x][A[l].y]=A[l].name[0];
							map[A[l].ox][A[l].oy]='.';
						}
						if(A[l].hp==0)
						{
							fprintf(ofp,"%s can't move. Dead.\n",A[l].name);
						}
					}
				}
			}
													
			fprintf(ofp,"\n");
		}
		if(strcmp(COM[i].c1,"ATTACK")==0 && strcmp(COM[i+1].c1,"HERO")==0)	 //attack command is performed for heroes
		{
			fprintf(ofp,"HEROES ATTACKED\n\n");
			
			for(m=0;m<nHERO;m++)
			{
				for(l=nHERO;l<nHERO+nMONSTER;l++)
				{
					int distance=((A[m].x-A[l].x)*(A[m].x-A[l].x)+(A[m].y-A[l].y)*(A[m].y-A[l].y));	
					
					if(distance<=2)
					{
						if(A[m].hp>0 && A[l].hp>0)
						{
							A[l].hp-=A[m].damage;
							
							if(A[l].hp<=0)
							{
								A[l].damage=0;
								A[l].hp=0;
								A[m].xp+=1;
								map[A[l].x][A[l].y]='.';
								dead_MONSTER++;
							}
						}				
					}
				}	
			}	
		}
		if(strcmp(COM[i].c1,"ATTACK")==0 && strcmp(COM[i+1].c1,"MONSTER")==0)      //attack command is performed for monsters
		{
			fprintf(ofp,"MONSTER ATTACKED\n\n");
			
			for(m=nHERO;m<nHERO+nMONSTER;m++)
			{
				for(l=0;l<nHERO;l++)
				{
					int distance=((A[m].x-A[l].x)*(A[m].x-A[l].x)+(A[m].y-A[l].y)*(A[m].y-A[l].y));	
					
					if(distance<=2)
					{
						if(A[m].hp>0 && A[l].hp>0)
						{
							A[l].hp-=A[m].damage;
							if(A[l].hp<=0)
							{
								A[l].damage=0;
								A[l].hp=0;
								map[A[l].x][A[l].y]='.';
								dead_HERO++;
							}
						}				
					}		
				}	
			}
		}
		if(strcmp(COM[i].c1,"SHOW")==0 && strcmp(COM[i+1].c1,"HERO")==0)		 //show command is performed for heros
		{
			fprintf(ofp,"HERO STATUS\n");
			for(l=0;l<nHERO;l++)
			{
				fprintf(ofp,"%s HP:%d XP:%d\n",A[l].name,A[l].hp,A[l].xp);
			}
			fprintf(ofp,"\n");
		}
		if(strcmp(COM[i].c1,"SHOW")==0 && strcmp(COM[i+1].c1,"MONSTER")==0)		//show command is performed for monsters
		{
			l=0;
			fprintf(ofp,"MONSTER STATUS\n");
			for(l+=nHERO;l<nMONSTER+nHERO;l++)
			{
				fprintf(ofp,"%s HP:%d\n",A[l].name,A[l].hp);
			}
			fprintf(ofp,"\n");
			
		}
		if(strcmp(COM[i].c1,"SHOW")==0 && strcmp(COM[i+1].c1,"MAP")==0)			//show command is performed for map
		{
			fprintf(ofp,"MAP STATUS\n");
			for (a=0;a<r;a++)
     		{
     			for (b=0;b<c;b++)
         		{
					fprintf(ofp,"%c ", map[a][b]);		
				}
				fprintf(ofp,"\n");
			}
			fprintf(ofp,"\n");	
		}
		if(dead_HERO==nHERO)
		{
			fprintf(ofp,"ALL HERO ARE DEAD!");
			break;
		}
		
		if(dead_MONSTER==nMONSTER)
		{
			fprintf(ofp,"ALL MONSTERS ARE DEAD!");
			break;
		}
	}

	fclose(i2fp);							//closing the files
	fclose(ifp);
	fclose(ofp);
	free(A);								//emptying memory which is dynamically allocated
	free(COM);
	for (a=0; a<r; a++)
        {
        free(map[a]);
	}
	
	return 0;
}
	
struct CHARS* expand(struct CHARS* A, int newsize)		//this function expand the memory when it is insufficient
{
	 return (struct CHARS*)realloc(A,newsize*sizeof(struct CHARS));
}


