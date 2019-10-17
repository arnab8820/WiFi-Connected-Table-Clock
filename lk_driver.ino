//convert integer to hex form compatible with lk1668
unsigned char c2h(int d)
{
  switch(d)
  {
    case 0: return(0xfc);
            break;
    case 1: return(0x60);
            break;
    case 2: return(0xda);
            break;
    case 3: return(0xf2);
            break;
    case 4: return(0x66);
            break;
    case 5: return(0xb6);
            break;
    case 6: return(0xbe);
            break;
    case 7: return(0xe0);
            break;
    case 8: return(0xfe);
            break;
    case 9: return(0xf6);
            break;
  }
}

//send dislay data
void senddata(unsigned char dat)
{
  unsigned char i ;
  for (i=0;i<8;i++)
  {
    digitalWrite(CLK, LOW);
    if(dat&0x01)
      digitalWrite(DIO, HIGH);
    else
      digitalWrite(DIO, LOW);
    digitalWrite(CLK,HIGH);
    dat=dat>>1;
  }
}

//send display command
void command(unsigned char com)
{
  digitalWrite(STB, HIGH);
  //delay(10);
  digitalWrite(STB, LOW);
  senddata(com);
}

//clear display
void clear_display()
{
  for(int x=0;x<14;x++)
  {
    disp_buffer[x]=0x00;
  }
  unsigned char i, j=0x00;
  command(0x03);    // Set to 7 * 10 mode
  command(0x30);    // Set the data command, using the address automatically increase 1 mode
  command(0x03);    // Set the display address, starting from 00H
  for(i=0;i<14;i++)
  {
    senddata(disp_buffer[i]);
  }
  command(0x8f);    // Show control command, open the display and set to the brightest
  digitalWrite(STB, HIGH);
}

//display data in 7 segment display
void display_data()
{
  command(0x03);    // Set to 7 * 10 mode
  command(0x40);    // Set the data command, using the address automatically increase 1 mode
  command(0xc0);    // Set the display address, starting from 00H
  for(int i=0;i<14;i++)
  {
    senddata(disp_buffer[i]);
  }
  command(0x8f);    // Show control command, open the display and set to the brightest
  digitalWrite(STB, HIGH);
}
