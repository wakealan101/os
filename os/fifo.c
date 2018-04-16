#include<stdio.h>
int main()
{
int i,j,n,a[50],f[10],no,z,available,count=0;
            printf("\n ENTER THE NUMBER OF PAGES:\n");
scanf("%d",&n);
            printf("\n ENTER THE PAGE NUMBER :\n");
            for(i=1;i<=n;i++)
            scanf("%d",&a[i]);
            printf("\n ENTER THE NUMBER OF fS :");
            scanf("%d",&no);
for(i=0;i<no;i++)
            f[i]= -1;
                        j=0;
                        printf("\tref string\t page fs\n");
for(i=1;i<=n;i++)
                        {
                                    printf("%d\t\t",a[i]);
                                    available=0;
                                    for(z=0;z<no;z++)
if(f[z]==a[i])
                                                available=1;
                                    if (available==0)
                                    {
                                                f[j]=a[i];
                                                j=(j+1)%no;
                                                count++;
                                                for(z=0;z<no;z++)
                                                printf("%d\t",f[z]);
}
                                    printf("\n");
}
                        printf("Page Fault Is %d",count);
                        return 0;
}
