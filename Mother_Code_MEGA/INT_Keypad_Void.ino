double INTType (char a, double b) {

  switch (a) {
    case '1' : Tnum = 1; b = (b * 10) + Tnum;    break;
    case '2' : Tnum = 2; b = (b * 10) + Tnum;    break;
    case '3' : Tnum = 3; b = (b * 10) + Tnum;    break;
    case '4' : Tnum = 4; b = (b * 10) + Tnum;    break;
    case '5' : Tnum = 5; b = (b * 10) + Tnum;    break;
    case '6' : Tnum = 6; b = (b * 10) + Tnum;    break;
    case '7' : Tnum = 7; b = (b * 10) + Tnum;    break;
    case '8' : Tnum = 8; b = (b * 10) + Tnum;    break;
    case '9' : Tnum = 9; b = (b * 10) + Tnum;    break;
    case '0' : b *= 10;                          break;

    
    case 'A' : Tnum = int(b) % 10; b = (b - Tnum) / 10; break;
    case 'B' : switch (EnterDataState){
                case 1 : ASize = 0; ASizeI=0; ASizeD=0; break;
                case 2 : AFlowrate = 0; AFlowrateD=0; AFlowrateI=0; break;
                case 3 : AVol = 0; AVolD=0; AVolI=0; break;         
               } 
               b=0; DivStat = 10; break;
    
    case 'C' : if(EnterDataState==1){
                AFlowrate = 0; AFlowrateD=0; AFlowrateI=0; 
                AVol = 0; AVolD=0; AVolI=0;
                b=ASizeI; DivStat = 10; DecimalPointStat = 0;
               }


               if(EnterDataState==2){
                AFlowrate = 0; AFlowrateD=0; AFlowrateI=0; 
                AVol = 0; AVolD=0; AVolI=0;
                b=0; DivStat = 10; DecimalPointStat = 0;
               }

               if(EnterDataState==3){
                AFlowrate = 0; AFlowrateD=0; AFlowrateI=0; 
                AVol = 0; AVolD=0; AVolI=0;
                b=0; DivStat = 10; DecimalPointStat = 0;
               }
               EnterDataState=2;
               break;

    
    case 'D' : if (EnterDataState >> 1) { EnterDataState--; }
               if (EnterDataState <= 1) { EnterDataState=1; } break;

               
    case '*' : DecimalPointStat = 1; break;

    
    case '#' : EnterDataState++; DivStat = 10.00; break;
  }
  return b;
}
