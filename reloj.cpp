#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
using namespace std;

class frame
{
public:
   int PageID;
   bool DirtyBit;
   bool RefBit;
   int PinCount;

   frame()
   {
      PageID = 0;
      DirtyBit = RefBit = 0;
      PinCount = 0;
   }
};

class reloj
{
   frame* BufferPool;
   int ClockHand;
   int cantframes;

public:
   reloj(int f)
   {
      cantframes = f;
      BufferPool = new frame[cantframes];
      ClockHand = cantframes-1;
   }

   int search(int n)
   {
      for (int i = 0; i < cantframes; i++)
      {
         if (BufferPool[i].PageID == n) return i;
      }
      return -1;
   }

   void addPage(int n)
   {
      int temp = search(n);
      if (temp != -1)
      {
         BufferPool[temp].PinCount++;
         BufferPool[temp].RefBit = 1;
         return;
      }
      
      for (int i = 0; i < cantframes; i++)
      {
         if (BufferPool[i].PinCount == 0) break;
         if (i == cantframes-1)
         {
            std::cout << "ALERTA! Buffer lleno! Termine uno de sus procesos.\n";
            return;
         }
      }

      while (true)
      {
         ClockHand++;
         ClockHand = ClockHand%cantframes;
         if (BufferPool[ClockHand].RefBit == 0)
         {
            BufferPool[ClockHand].PageID = n;
            BufferPool[ClockHand].RefBit = 1;
            BufferPool[ClockHand].PinCount = 1;
            BufferPool[ClockHand].DirtyBit = 0;
            break;
         }
         BufferPool[ClockHand].RefBit = 0;
      }
   }

   void printPool()
   {
      std::cout << "CLOCKHAND: " << ClockHand;
      for (int i = 0; i < cantframes; i++)
      {
         std::cout << " | "<< BufferPool[i].PageID << "-" << BufferPool[i].RefBit;
      }
      std::cout << std::endl;
   }
};


int main()
{
   reloj AC(4);
   int t[] = {1,2,4,4,5,1,5,3,1,2,3,1,5};
   
   for(int i = 0; i < 13; i++)
   {
      AC.addPage(t[i]);
      std::cout << char(t[i]+96) << ": ";
      AC.printPool();
   }
   return 0;
}