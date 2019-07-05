#include <cstdio>
#include <cstring>
#include <iostream>
#include <cmath>
#define rep(i,a,b) for (int i = a;i <= b;++i)
#define drep(i,a,b) for (int i = a;i >= b;--i)
#define ist insert
#define ers erase
#define clr(a,b) memset(a,b,sizeof a)
#define ll long long
#define inf 2000000000
#define zhl 1000000007
template<class T>void Swap(T &a,T &b){ T c = a;a = b;b = c; }
template<class T>T max(T& a,T& b){ return a<b?b:a; }
template<class T>T min(T& a,T& b){ return a>b?b:a; }
using namespace std;

void IF() {}
void ID() {}
void EX() {}
void MEM() {}
void WB() {}

int pc,a[150000],now,S[32];
char s1[20],s2[20],s3[20],s4[20];
int Trans(char s[])
{
	int len = strlen(s),ans = 0;
	for (int i = 0;i < len;i++)
		if (s[i] >= '0' && s[i] <= '9') ans = ans*16+s[i]-'0';
		else if (s[i] >= 'A' && s[i] <= 'F') ans = ans*16+s[i]-'A'+10;
	return ans;
}
int GetCode(int x,int l,int r)
{
	return (x>>l)&((1<<(r-l+1))-1);
}
void PrintBin(int x)
{
	int b[32],d = 0,tmp = x;
	for (int i = 0;i < 32;i++) b[i] = 0;
	if (x < 0) b[31] = 1, x = -x;
	for (;x;x >>= 1) b[d++] = x&1;
	for (int i = 31;i >= 0;i--) printf("%d",b[i]);
	puts("");
	for (int i = 31;i >= 0;i--) printf("%d",GetCode(tmp,i,i));
	puts("");
	
	return;
}

int Expand(int x,int g)
{
	if (x&(1<<(g-1))) x |= ((1<<(32-g))-1)<<g;
	return x;
}
int GetMemory(int pc,int d)
{
	int ans = 0;
	for (int i = 0;i <= d-1;i++)
		ans += a[pc+i]<<(i*8);
	return ans;
}
void Load()
{
	int funct = GetCode(now,12,14);
	int rd = GetCode(now,7,11);
	int rs1 = GetCode(now,15,19);
	int imm = GetCode(now,20,31);
	imm = Expand(imm,12);
	rs1 = S[rs1];
//	printf("L:%d %d\n",rs1,imm);
	if (funct == 0) //lb
		S[rd] = Expand(GetMemory(rs1+imm,1),8);
	else if (funct == 1)
		S[rd] = Expand(GetMemory(rs1+imm,2),16);
	else if (funct == 2)
		S[rd] = GetMemory(rs1+imm,4);
	else if (funct == 4)
		S[rd] = GetMemory(rs1+imm,1);
	else if (funct == 5)
		S[rd] = GetMemory(rs1+imm,2);
//	printf("Load : %d %d %d %d\n",rd,S[rd],rs1+imm,a[rs1+imm]);
	pc += 4;
}
void Store()
{
	int funct = GetCode(now,12,14);
	int rd = GetCode(now,20,24);
	int rs1 = GetCode(now,15,19);
	int imm = GetCode(now,7,11)+(GetCode(now,25,31)<<5);
	imm = Expand(imm,12);
	rs1 = S[rs1];
	if (funct == 0)
		a[rs1+imm] = GetCode(S[rd],0,7);
	else if (funct == 1)
		a[rs1+imm] = GetCode(S[rd],0,7),
		a[rs1+imm+1] = GetCode(S[rd],8,15);
	else a[rs1+imm] = GetCode(S[rd],0,7),
		a[rs1+imm+1] = GetCode(S[rd],8,15),
		a[rs1+imm+2] = GetCode(S[rd],16,23),
		a[rs1+imm+3] = GetCode(S[rd],24,31); 
	pc += 4;
//	printf("Store : %d %d %d\n",rs1+imm,a[rs1+imm],S[rd]);
}
void Add()
{
	int funct = GetCode(now,12,14);
	int rd = GetCode(now,7,11);
	int rs1 = GetCode(now,15,19);
	int rs2 = GetCode(now,20,24);
	int funct1 = GetCode(now,25,31);
	if (funct == 0) //add sub
	{
		if (!funct1) S[rd] = S[rs1]+S[rs2];
		else S[rd] = S[rs1]-S[rs2];
	}
	else if (funct == 1) //sll
		S[rd] = S[rs1]<<(GetCode(S[rs2],0,4));
	else if (funct == 2) // slt
	{
		if (S[rs1]<S[rs2]) S[rd] = 1;
	}
	else if (funct == 3) //sltu
	{
		if ((unsigned int)S[rs1] < (unsigned int)S[rs2]) S[rd] = 1;
	}
	else if (funct == 4) // xor
		S[rd] = S[rs1]^S[rs2];
	else if (funct == 5) //srl sra
	{
		if (funct1) S[rd] = S[rs1]>>(GetCode(S[rs2],0,4));
		else
		{
			int t = GetCode(S[rs2],0,4);
			if (S[rs1]&(1<<31)) S[rd] = (S[rs1]>>t)&((1<<(32-t))-1);
			else S[rd] = S[rs1]>>t;
		}
	}
	else if (funct == 6)
		S[rd] = S[rs1]|S[rs2];
	else if (funct == 7)
		S[rd] = S[rs1]&S[rs2];
	pc += 4;
}
void Addi()
{
	int funct = GetCode(now,12,14);
	int rd = GetCode(now,7,11);
	int rs1 = GetCode(now,15,19);
	int imm = GetCode(now,20,31);
	imm = Expand(imm,12);
	if (funct == 0)
		S[rd] = S[rs1]+Expand(imm,12);
	else if (funct == 1)
		S[rd] = S[rs1]<<GetCode(imm,0,4);
	else if (funct == 2)
	{
		if (S[rs1] < imm) S[rd] = 1;
	}
	else if (funct == 3)
	{
		if ((unsigned int)S[rs1] < (unsigned int)imm) S[rd] = 1;
	}
	else if (funct == 4)
		S[rd] = S[rs1]^imm;
	else if (funct == 5)
	{
		imm = GetCode(imm,0,4);
		if (GetCode(now,25,31)) S[rd] = S[rd]>>imm;
		else if (S[rs1]&(1<<31)) S[rd] = (S[rs1]>>imm)&((1<<(32-imm))-1);
			else S[rd] = S[rs1]>>imm;
	}
	else if (funct == 6)
		S[rd] = S[rs1]|imm;
	else if (funct == 7)
		S[rd] = S[rs1]&imm;
	pc += 4;
}
void Branch()
{
	int funct = GetCode(now,12,14);
	int rs1 = GetCode(now,15,19);
	int rs2 = GetCode(now,20,24);
	int imm = (GetCode(now,7,7)<<11)+(GetCode(now,8,11)<<1)+(GetCode(now,25,30)<<5)+(GetCode(now,31,31)<<12);
	imm = Expand(imm,13);
//	printf("Imm:%d %d %d %d %d\n",imm,rs1,S[rs1],rs2,S[rs2]);
	if (funct == 0) pc += (S[rs1]==S[rs2]?imm:4);
	else if (funct == 1) pc += (S[rs1]!=S[rs2]?imm:4);
	else if (funct == 4) pc += (S[rs1]<S[rs2]?imm:4);
	else if (funct == 5) pc += (S[rs1]>=S[rs2]?imm:4);
	else if (funct == 6) pc += ((unsigned int)S[rs1]<(unsigned int)S[rs2]?imm:4);
	else if (funct == 7) pc += ((unsigned int)S[rs1]>=(unsigned int)S[rs2]?imm:4);
}
void Lui()
{
	int rd = GetCode(now,7,11);
	int imm = GetCode(now,12,31);
	S[rd] = imm<<12;
	pc += 4;
}
void Auipc()
{
	int rd = GetCode(now,7,11);
	int imm = GetCode(imm,12,31)<<12;
	pc += imm;
//	S[rd] = GetMemory(pc,4);
	S[rd] = pc;
}
void Jal()
{
	int rd = GetCode(now,7,11);
	int imm = (GetCode(now,12,19)<<12)+(GetCode(now,20,20)<<11)+(GetCode(now,21,30)<<1)+(GetCode(now,31,31)<<20);
	imm = Expand(imm,21);
//	S[rd] = GetMemory(pc+4,4);
//	printf("rd:%d %d\n",rd,pc);
	S[rd] = pc+4;
	pc = pc+imm;
}
void Jalr()
{
	int rd = GetCode(now,7,11);
	int rs1 = GetCode(now,15,19);
	int imm = Expand(GetCode(now,20,31),12);
	S[rd] = pc+4;
	pc = S[rs1]+imm;
}
int K;
bool excute()
{
	now = GetMemory(pc,4);
	if (now == 0x00c68223)
	{
		printf("%d\n",(unsigned int)S[10]&255u);
		return 1;
	}
	int opcode = GetCode(now,0,6);
	
//	printf("%d %d %d\n",++K,pc,now);
	if (opcode == 3) Load();// Load
	else if (opcode == 35) Store();// Store
	else if (opcode == 51) Add();// Add
	else if (opcode == 19) Addi();// AddI
	else if (opcode == 99) Branch();// BEQ
	else if (opcode == 55) Lui();// LUI
	else if (opcode == 23) Auipc();// AUIPC
	else if (opcode == 111) Jal();// JAL
	else if (opcode == 103) Jalr();// JALR
	else return 1;
	S[0] = 0;
	return 0;
}
int main()
{
	freopen("1.data","r",stdin);
	for (;scanf("%s",s1)!=EOF;)
	{
		if (s1[0] == '@')
		{
			pc = 0;
			for (int i = 1;i <= 8;i++)
				if (s1[i] >= '0' && s1[i] <= '9') pc = pc*16+s1[i]-'0';
				else pc = pc*16+s1[i]-'A'+10;
		}
		else
		{
			scanf("%s%s%s",s2,s3,s4);
			a[pc++] = Trans(s1);
			a[pc++] = Trans(s2);
			a[pc++] = Trans(s3);
			a[pc++] = Trans(s4);
		}
	}
	pc = 0; 
	for (;;)
		if (excute()) break;
/*	for (;;)
	{
		WB();
		MEM();
		EX();
		ID();
		IF();
	}*/
}
