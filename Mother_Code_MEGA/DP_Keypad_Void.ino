double DPType(char a , double d) {

  switch (a) {
    case '1' : Tnum = 1; d = d + (Tnum / DivStat); DivStat *= 10; break;
    case '2' : Tnum = 2; d = d + (Tnum / DivStat); DivStat *= 10; break;
    case '3' : Tnum = 3; d = d + (Tnum / DivStat); DivStat *= 10; break;
    case '4' : Tnum = 4; d = d + (Tnum / DivStat); DivStat *= 10; break;
    case '5' : Tnum = 5; d = d + (Tnum / DivStat); DivStat *= 10; break;
    case '6' : Tnum = 6; d = d + (Tnum / DivStat); DivStat *= 10; break;
    case '7' : Tnum = 7; d = d + (Tnum / DivStat); DivStat *= 10; break;
    case '8' : Tnum = 8; d = d + (Tnum / DivStat); DivStat *= 10; break;
    case '9' : Tnum = 9; d = d + (Tnum / DivStat); DivStat *= 10; break;
    case '0' : DivStat *= 10;                                     break;

    
    case 'A' : if (DivStat >= 11) { DivStat /= 10; } 
               d = d - (float(int(d * DivStat) % 10) / DivStat); break;

               
    case 'B' : switch (EnterDataState){
                case 1 : ASize = 0; ASizeI=0; ASizeD=0; break;
                case 2 : AFlowrate = 0; AFlowrateD=0; AFlowrateI=0; break;
                case 3 : AVol = 0; AVolD=0; AVolI=0; break;      
               } 
               d=0; DivStat = 10; DecimalPointStat = 0; break;
               
    case 'C' : if(EnterDataState==1){
                AFlowrate = 0; AFlowrateD=0; AFlowrateI=0; 
                AVol = 0; AVolD=0; AVolI=0;
                d=ASizeD; DivStat = 10; DecimalPointStat = 0;
               }

               if(EnterDataState==2){
                AFlowrate = 0; AFlowrateD=0; AFlowrateI=0; 
                AVol = 0; AVolD=0; AVolI=0;
                d=0; DivStat = 10; DecimalPointStat = 0;
               }

               if(EnterDataState==3){
                AFlowrate = 0; AFlowrateD=0; AFlowrateI=0; 
                AVol = 0; AVolD=0; AVolI=0;
                d=0; DivStat = 10; DecimalPointStat = 0;
               }
               EnterDataState=2;
               break;
               
    case 'D' : if (EnterDataState >> 1) { EnterDataState--; }
               if (EnterDataState <= 1) { EnterDataState=1; } 
               DecimalPointStat = 0; break;
               
    case '*' : DecimalPointStat = 0; break; //Decimal Point
    
    case '#' : EnterDataState++; DivStat = 10.00; DecimalPointStat = 0; break;
  }
  return d;

}
