#include "TMath.h"
#include "iostream"
#include "TString.h"
#include "fstream"
#include "TBox.h"
#include "TIterator.h"
#include "TList.h"
#include "TCanvas.h"
#include "TMap.h"
#include "TParameter.h"
#include "TObjString.h"

#include "FlagGenerator.h"

int ViableStyles(int stars = 51, bool verbose = 0, int graphics = 2, FILE* flagcsv = NULL){
  //graphics = 0 None
  //         = 1 canton only
  //         = 2 full flag

  //int rc = 0;
  int rows = 1;
  int columns = 1;
  int numflags = 0;

  double flagheight = 1000;
  double widthoverheight = 1.9;
  double flagwidth = widthoverheight*flagheight;

  double stripeheight = flagheight*(1/13.0);

  double cantonheight = stripeheight*7.0;
  double cantonwidth = flagwidth*0.4;

  // 0 COStyle = stars + 2;
  // 1 oddcheck1 = (stars*2) -1;
  // 2 oddcheck2 = (stars*2) +1;
  // 3 evencheck = stars*2;
  // 4 grid = stars;

  TMap* StarNumMap = new TMap();
  // TMap* StyleNameMap = new TMap();
  TMap* StyleDescMap = new TMap();
  TMap* BasisMap = new TMap();

  //Colorado
  TString stname = "Colorado";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars + 2));
  StyleDescMap->Add(new TObjString(stname),new TObjString("Colorado"));
  BasisMap->Add(new TObjString(stname),new TObjString("Grid"));

  stname = "ColoradoM4";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars + 6));
  StyleDescMap->Add(new TObjString(stname),new TObjString("ColoradoM4"));
  BasisMap->Add(new TObjString(stname),new TObjString("Grid"));

  //Oklahoma (-1 star from 2nd and 2nd to last rows) stars = rc - 2 => rc = stars +2
  stname = "Oklahoma";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars + 2));
  StyleDescMap->Add(new TObjString(stname),new TObjString(stname));
  BasisMap->Add(new TObjString(stname),new TObjString("Grid"));

  //NC
  stname = "NC";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars + 4));
  StyleDescMap->Add(new TObjString(stname),new TObjString(stname));
  BasisMap->Add(new TObjString(stname),new TObjString("Grid"));

  //Oregon
  stname = "Oregon";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars + 2));
  StyleDescMap->Add(new TObjString(stname),new TObjString(stname));
  BasisMap->Add(new TObjString(stname),new TObjString("Grid"));

  //Odd Checkerboard with star corner
  stname = "CheckStar";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname, (stars*2) -1));
  StyleDescMap->Add(new TObjString(stname),new TObjString("Odd Checkerboard star corner"));
  BasisMap->Add(new TObjString(stname),new TObjString("Checkerboard"));

  //Odd Checkerboard with blue corner
  stname = "CheckBorder";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname, (stars*2) - 3));
  StyleDescMap->Add(new TObjString(stname),new TObjString(stname));
  BasisMap->Add(new TObjString(stname),new TObjString("Checkerboard"));

  //Checkerboard with border
  stname = "CheckBlue";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname, (stars*2) +1));
  StyleDescMap->Add(new TObjString(stname),new TObjString("Odd Checkerboard blue corner"));
  BasisMap->Add(new TObjString(stname),new TObjString("Checkerboard"));

  //Even Checkerboard
  stname = "CheckEven";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname, stars*2));
  StyleDescMap->Add(new TObjString(stname),new TObjString("Even Checkerboard"));
  BasisMap->Add(new TObjString(stname),new TObjString("Checkerboard"));

  //South Dakota
  //rc/2 + 1 = stars (we dont count the left-most column in the rc number)
  //rc/2 = stars -1
  //rc = (stars - 1)*2
  // (6 * 14)/2 + 1 = 43 
  stname = "SD";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname, (stars-1)*2));
  StyleDescMap->Add(new TObjString(stname),new TObjString(stname));
  BasisMap->Add(new TObjString(stname),new TObjString("Checkerboard2"));

  stname = "SD2";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname, (stars-1)*2));
  StyleDescMap->Add(new TObjString(stname),new TObjString(stname));
  BasisMap->Add(new TObjString(stname),new TObjString("Checkerboard2"));

  //Utah
  stname = "Utah";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname, stars*2));
  StyleDescMap->Add(new TObjString(stname),new TObjString("stname"));
  BasisMap->Add(new TObjString(stname),new TObjString("Checkerboard"));

  //Even Checkerboard - Inverted
  stname = "CheckEvenInv";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname, stars*2));
  StyleDescMap->Add(new TObjString(stname),new TObjString("Even Checkerboard inverted"));
  BasisMap->Add(new TObjString(stname),new TObjString("Checkerboard"));

  //Arkansas: rc - 3 = stars => rc = stars +3
  stname = "Arkansas";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars+3));
  StyleDescMap->Add(new TObjString(stname),new TObjString("Arkansas"));
  BasisMap->Add(new TObjString(stname),new TObjString("Grid"));


  //Simple Grid
  stname = "Grid";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars));
  StyleDescMap->Add(new TObjString(stname),new TObjString("Grid"));
  BasisMap->Add(new TObjString(stname),new TObjString("Grid"));

  //Simple Diamond
  stname = "Diamond";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars));
  StyleDescMap->Add(new TObjString(stname),new TObjString(stname));
  BasisMap->Add(new TObjString(stname),new TObjString("Grid"));

  //Kansas
  stname = "Kansas";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars+1));
  StyleDescMap->Add(new TObjString(stname),new TObjString("Kansas"));
  BasisMap->Add(new TObjString(stname),new TObjString("Grid"));

  //AL_ME
  stname = "AL_ME";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars+1));
  StyleDescMap->Add(new TObjString(stname),new TObjString("AL_ME"));
  BasisMap->Add(new TObjString(stname),new TObjString("Grid"));

  //Michigan (Wyoming)
  stname = "Michigan";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars-2));
  StyleDescMap->Add(new TObjString(stname),new TObjString(stname));
  BasisMap->Add(new TObjString(stname),new TObjString("Grid2"));

  //California (31)
  //rc = 5x6 = 30 = stars - 1
  stname = "California";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars-1));
  StyleDescMap->Add(new TObjString(stname),new TObjString(stname));
  BasisMap->Add(new TObjString(stname),new TObjString("Grid2"));

  //Nebraska
  //If we use full grid, rows need to be odd => stars = rc - ((rows+1)/2)
  //If partial grid = rc + (rows - 1)/2
  stname = "Nebraska";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars));
  StyleDescMap->Add(new TObjString(stname),new TObjString(stname));
  BasisMap->Add(new TObjString(stname),new TObjString("Grid"));

  stname = "Colorado2";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars));
  StyleDescMap->Add(new TObjString(stname),new TObjString(stname));
  BasisMap->Add(new TObjString(stname),new TObjString("Grid"));

  //Nevada
  stname = "Nevada";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars-1));
  StyleDescMap->Add(new TObjString(stname),new TObjString(stname));
  BasisMap->Add(new TObjString(stname),new TObjString("Grid2"));

  //Illinois
  stname = "Illinois";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars+3));
  StyleDescMap->Add(new TObjString(stname),new TObjString(stname));
  BasisMap->Add(new TObjString(stname),new TObjString("Grid"));

  //Nevada - 4
  stname = "NevadaM4";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars+3));
  StyleDescMap->Add(new TObjString(stname),new TObjString(stname));
  BasisMap->Add(new TObjString(stname),new TObjString("Grid2"));

  //Nevada - 8
  stname = "NevadaM8";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars+7));
  StyleDescMap->Add(new TObjString(stname),new TObjString(stname));
  BasisMap->Add(new TObjString(stname),new TObjString("Grid2"));

  //Pentagonal number with center
  stname = "Circle_Pent";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars));
  StyleDescMap->Add(new TObjString(stname),new TObjString("Circle_Pent"));
  BasisMap->Add(new TObjString(stname),new TObjString("Circle"));

  //Hexagonal number with center
  stname = "Circle_Hex";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars));
  StyleDescMap->Add(new TObjString(stname),new TObjString(stname));
  BasisMap->Add(new TObjString(stname),new TObjString("Circle"));

  stname = "Circle_Hex_Flex";
  StarNumMap->Add(new TObjString(stname),new TParameter<int>(stname,stars));
  StyleDescMap->Add(new TObjString(stname),new TObjString(stname));
  BasisMap->Add(new TObjString(stname),new TObjString("Circle"));
  

  //int rc[6] = {stars + 2, (stars*2) -1,  (stars*2) +1, stars*2, stars, stars + 1};
  Int_t rc;
  Int_t circle_var;
  Int_t circle_var_max = 20;

  //[num_stars], ring 0, 1, 2, 3, 4
  Int_t ring_hex[65][5];
  for(int i = 0; i<65;i++){ for(int j = 0; j<5; j++){ring_hex[i][j] = - 1;}}//Initialize all to -1
  //0	0	11	0	0	11
 ring_hex[11][0]=0;	ring_hex[11][1]=0;	ring_hex[11][2]=11;	ring_hex[11][3]=0;	ring_hex[11][4]=0;
 ring_hex[12][0]=0;	ring_hex[12][1]=0;	ring_hex[12][2]=12;	ring_hex[12][3]=0;	ring_hex[12][4]=0;
 ring_hex[13][0]=0;	ring_hex[13][1]=0;	ring_hex[13][2]=13;	ring_hex[13][3]=0;	ring_hex[13][4]=0;
 // ring_hex[13][0]=1;	ring_hex[13][1]=0;	ring_hex[13][2]=12;	ring_hex[13][3]=0;	ring_hex[13][4]=0;
 ring_hex[14][0]=1;	ring_hex[14][1]=0;	ring_hex[14][2]=13;	ring_hex[14][3]=0;	ring_hex[14][4]=0;
 ring_hex[17][0]=0;	ring_hex[17][1]=6;	ring_hex[17][2]=11;	ring_hex[17][3]=0;	ring_hex[17][4]=0;
 //ring_hex[18][0]=0;	ring_hex[18][1]=6;	ring_hex[18][2]=12;	ring_hex[18][3]=0;	ring_hex[18][4]=0;
 ring_hex[19][0]=0;	ring_hex[19][1]=6;	ring_hex[19][2]=13;	ring_hex[19][3]=0;	ring_hex[19][4]=0;
 //ring_hex[19][0]=1;	ring_hex[19][1]=6;	ring_hex[19][2]=12;	ring_hex[19][3]=0;	ring_hex[19][4]=0;
 ring_hex[20][0]=1;	ring_hex[20][1]=6;	ring_hex[20][2]=13;	ring_hex[20][3]=0;	ring_hex[20][4]=0;
 ring_hex[29][0]=0;	ring_hex[29][1]=0;	ring_hex[29][2]=11;	ring_hex[29][3]=18;	ring_hex[29][4]=0;
 ring_hex[30][0]=1;	ring_hex[30][1]=0;	ring_hex[30][2]=11;	ring_hex[30][3]=18;	ring_hex[30][4]=0;
 ring_hex[31][0]=0;	ring_hex[31][1]=0;	ring_hex[31][2]=12;	ring_hex[31][3]=19;	ring_hex[31][4]=0;
 ring_hex[32][0]=1;	ring_hex[32][1]=0;	ring_hex[32][2]=13;	ring_hex[32][3]=18;	ring_hex[32][4]=0;
 // ring_hex[33][0]=0;	ring_hex[33][1]=0;	ring_hex[33][2]=13;	ring_hex[33][3]=0;	ring_hex[33][4]=20;
 //ring_hex[33][0]=1;	ring_hex[33][1]=0;	ring_hex[33][2]=12;	ring_hex[33][3]=0;	ring_hex[33][4]=20;
 //ring_hex[33][0]=1;	ring_hex[33][1]=0;	ring_hex[33][2]=13;	ring_hex[33][3]=19;	ring_hex[33][4]=0;
 ring_hex[34][0]=1;	ring_hex[34][1]=0;	ring_hex[34][2]=13;	ring_hex[34][3]=0;	ring_hex[34][4]=20;
 //ring_hex[35][0]=0;	ring_hex[35][1]=0;	ring_hex[35][2]=11;	ring_hex[35][3]=0;	ring_hex[35][4]=24;
 //ring_hex[35][0]=0;	ring_hex[35][1]=6;	ring_hex[35][2]=11;	ring_hex[35][3]=18;	ring_hex[35][4]=0;
 //ring_hex[36][0]=0;	ring_hex[36][1]=6;	ring_hex[36][2]=12;	ring_hex[36][3]=18;	ring_hex[36][4]=0;
 ring_hex[37][0]=0;	ring_hex[37][1]=0;	ring_hex[37][2]=13;	ring_hex[37][3]=0;	ring_hex[37][4]=24;
 //ring_hex[37][0]=1;	ring_hex[37][1]=6;	ring_hex[37][2]=12;	ring_hex[37][3]=18;	ring_hex[37][4]=0;;


  //string name[6] = {"Colorado","Odd Checkerboard star corner","Odd checkerboard blue corner","Even checkerboard", "Grid", "Kansas"};
  //char* command[6] = {
  //  "Colorado",
  // "CheckStar",
  // "CheckBlue",
  // "CheckEven",
  // "Grid",
  // "Kansas"
  //};
  char* conter_char;
  TString canvasname = "";
  
  if(flagcsv) fprintf(flagcsv,"%i",stars);
  //printf("%% %i STARS \n",stars);

  if(verbose) cout<<"Pincer 1.0"<<endl;

  TIterator* the_iter = StarNumMap->MakeIterator();
  TObjString* counter_str_obj = new TObjString();
  TString counter_str;
  TString basis;

  int n = -1;
  int cpn = 0;
  int chn = 0;
  int chf = 0;
  int chf_best = 0;
  int chf_pass[6];
      
  // counter_str_obj =(TObjString*)the_iter->Next();
  //counter_str = counter_str_obj->GetString();
  //cout<<"The string is: "<<counter_str<<endl;
  
  // Int_t rc2 = ((TParameter<int>*)StarNumMap->GetValue(counter_str))->GetVal();
  //cout<<"rows x columns = "<<rc2<<endl;
  //Grid 6x8 is OK 1.333
  //     6x9 is OK 1.5
  //     7x13 is OK 1.86
  //     4x8 is ?? (2.0)
  //     5x11 NOT 2.2
  //     4x13 NOT
  //     5x13 NOT
  //     4x17 NOT
  //     7x8 OK 1.14
  //for(int j = 0; j<6; j++){
  double max_ColumnToRow_ratio_grid = 2.0; //(13./7.0);
  double max_ColumnToRow_ratio_check = 5;
  double ideal_ColumnToRow_ratio = cantonwidth/cantonheight;
  double actual_ColumnToRow_ratio = 0;
  int num_ratios = 0;
  int prev_rows[2] = {0,0};
  int prev_cols[2] = {0,0};
  int remainder = -1;
  if(verbose) cout<<"Ideal Ratio = "<<ideal_ColumnToRow_ratio<<endl;
  //Issues
  // - column to row ration is not correctly caluclated for Nevada, California, and Michigan styles
  // - Some flags produced do not match historical ones

  while( ( counter_str_obj =(TObjString*)the_iter->Next() )){
    counter_str = counter_str_obj->GetString();
    if(verbose) cout<<endl<<"Checking for "<<counter_str<<endl;
    if(flagcsv) fprintf(flagcsv,",");
    rc = ((TParameter<int>*)StarNumMap->GetValue(counter_str))->GetVal();
    basis = ((TObjString*) BasisMap->GetValue(counter_str))->GetString();
    if(verbose) cout<<"Basis is "<<basis<<endl;
    rows = 1; columns = 1;
    prev_rows[0] = 0; prev_rows[1] =0; prev_cols[0] = 1; prev_cols[1] = 1;
    num_ratios = 0;
    //For Nebraska, stars = rc - ((rows+1)/2) (only works for odd rows)
    if(basis != "Circle" && counter_str != "Diamond"){
      for(int i = 1; i<=TMath::Sqrt(rc); i++){
	remainder = rc%i; //Default
	if(counter_str == "Nebraska") remainder = (2*rc + i + 1)%(2*i);
	if(counter_str == "Colorado2") remainder = (2*rc + i - 1)%(2*i);
	if(remainder == 0 ){//Even interger division
	  rows = i;
	  columns = rc/i;
	  if(counter_str == "Nebraska") columns = (2*rc + i + 1)/(2*i);
	  if(counter_str == "Colorado2") columns = (2*rc + i - 1)/(2*i);
	  //actual_ColumnToRow_ratio = (double)columns/rows;
	  if((basis == "Checkerboard" || basis=="Checkerboard2")  && columns <= max_ColumnToRow_ratio_check*rows) num_ratios++;
	  // if(verbose) {
	  //  cout<<"Columns = "<<columns<<"/"<<" Rows = "<<rows<<" = "<<actual_ColumnToRow_ratio<<endl;
	  // }
	  prev_rows[0] = prev_rows[1];
	  prev_cols[0] = prev_cols[1];
	  prev_rows[1] = rows;
	  prev_cols[1] = columns;
	}
      }
      if((basis == "Checkerboard" || basis=="Checkerboard2") && verbose) cout<<"Number of viable ratios = "<<num_ratios<<endl;
      //- this is the last time through this loop
      //- we have a good ratio prior to this one
	  //- this is checkerboard
	  //-- set rows and columns to previous rows and colums
      if(num_ratios > 1 && (counter_str == "Utah" || counter_str == "SD")){
	rows = prev_rows[0];
	columns = prev_cols[0];
      }
    } else if(counter_str == "Diamond"){
      rows = 0;
      columns = -1;
      int randc = -1;
      double rsq;
      int dn = 0;
      while(rc - dn >= 0){
	randc += 2;
	rsq = randc*randc;
	dn = rsq/2. + 0.5;
	if(verbose) cout<<"diamond number = "<<dn<<" = "<<rsq/2.<<" + "<<0.5<<endl;
      }
      randc = randc - 2;
      rsq = randc*randc;
      dn = (rsq)/2. + 0.5;
      if(verbose) cout<<"diamond number = "<<dn<<" = "<<rsq/2.<<" + "<<0.5<<endl;
      if(verbose) cout<<"Diamond Stars = "<<rc<<", dn("<<randc<<") = "<<dn<<endl;
      if((dn - rc)%4 == 0  && (dn - rc)<randc-1){
	rows = randc;
	columns = rc - dn;
      }
      if(verbose) cout<<"Rows = "<<rows<<", columns = "<<columns<<endl;

    } else if (counter_str == "Circle_Pent"){
      if(verbose) cout<<"In "<<counter_str<<" branch"<<endl;
      //rows becomes the number of rings
      //columns becomes 5 for pentagonal, 6 for hexagonal
      // for pentagonal - 1 (NCe NCo)
      // for pentagonal + 4 (WCe WCo)
      // for pentagonal + 3 (NCe WCo)
      rows = 0;
      // columns = 0;
      circle_var = -1000;
      n = -1;
      cpn = 0;
      columns = 5;
      while(rc - cpn >= -1){
	n++;
	cpn = (5*n*n + 5*n + 2)/2;
	// circle_var =rc-cpn;
	//	rows = n; //When we reach = -1, we can try to see if it works
      } //we have overshot n by 1 at the end of this loop
      cpn = (5*(n-1)*(n-1) + 5*(n-1) + 2)/2;
      circle_var =rc-cpn;
      rows = n-1; //When we reach = -1, we can try to see if it works
    } else if (counter_str == "Circle_Hex"){
      if(verbose) cout<<"In "<<counter_str<<" branch"<<endl;
      rows = 0;
      circle_var = -1000;
      n=-1;
      chn = 0;
      columns = 6;
      while(rc-chn >= -1){
	n++;
	chn = 3*n*(n-1) + 1;
      }
      chn = 3*(n-1)*(n-2) + 1;
      circle_var =rc-chn;
      rows = n-2; //Subtract an extra 1 since hexagonal numbers start counting at 1 rather than zero.
    } else if (counter_str == "Circle_Hex_Flex"){
      circle_var = -1000;
      chf = 11;
      while(rc - chf >= 0 && rc<60){ 
	//Start with a set of "core" numbers 11, 12, 13, 14, 17, 18, 19, 20, ...
	//Run through the core until we find the number that is closest to the desired number of stars, without going over
	if(verbose) cout<<"Checker for CHF = "<<chf<<endl;
	if(ring_hex[chf][0] != -1 && (rc-chf)%2==0) {
	  circle_var = rc - chf;
	  chf_best = chf;
	  if(verbose) cout<<"Best CHF so far is "<<chf_best<<", which is off from "<<rc<<" by "<<circle_var<<endl;
	}
	chf++;
	//If core = stars, use it
	//If core+20 >= stars, use with pillars
      }
      // chf_pass[0]  = chf-1;
      //rows = 5;
      for(int i = 0;i<5;i++) {
	chf_pass[i] = ring_hex[chf_best][i];
	if(ring_hex[chf_best][i] >0) rows = i;
      }
      columns = 6;
    }
    
    if(rows == 1) {
      if(verbose) cout<<rc<<" is a prime number; "<<counter_str<<" is not viable."<<endl;
    } else if ((basis=="Grid" && columns >  max_ColumnToRow_ratio_grid*rows) ||
	       (basis=="Checkerboard" && columns >  max_ColumnToRow_ratio_check*rows)) {// 
      if(verbose) {
	cout<<rows<<" x "<<columns<<" is too far off; "<<counter_str<<" is not viable."<<endl;
	if(basis == "Grid") cout<<"Max ratio is "<<max_ColumnToRow_ratio_grid<<", actual ratio is "<<(double)columns/rows<<endl;
	if(basis == "Checkerboard") cout<<"Max ratio is "<<max_ColumnToRow_ratio_check<<", actual ratio is "<<(double)columns/rows<<endl;
      }
    } else if (basis == "Grid2" && columns + 1 >  max_ColumnToRow_ratio_grid*rows){
      if(verbose) cout<<rows<<" x "<<columns + 1<<" is too far off; "<<counter_str<<" is not viable."<<endl;
    } else if (basis == "Checkerboard2" && columns+1 >  max_ColumnToRow_ratio_check*rows){
      if(verbose) cout<<rows<<" x "<<columns + 1<<" is too far off; "<<counter_str<<" is not viable."<<endl;
    } else if ((counter_str == "Utah" || counter_str == "SD") && num_ratios<=1){
      if(verbose) cout<<"Not enough viable ratios to dip to Utah or SD"<<endl;
    } else if (counter_str == "CheckBorder" && (rows%2 == 0 || columns%2 == 0) ){
      if(verbose) cout<<"Checkerboard with border only works with odd x odd checkerboard."<<endl;
    } else if (counter_str == "Kansas" && rows%2 == 0){
      if(verbose) cout<<"This is an even number of rows. Kansas only works with odd row numbers"<<endl;
    } else if (counter_str == "AL_ME" && rows%2 == 1){
      if(verbose) cout<<"This is an odd number of rows. Alabama-Maine only makes sense for an even number of rows. For an odd number, use Kansas."<<endl;
    } else if (counter_str == "Oklahoma" && rows < 5){
      if(verbose) cout<<"This is too few rows for Oklahoma to make sense."<<endl;
    } else if (counter_str == "Arkansas" && rows < 3){
      if(verbose) cout<<"This is too few rows for Arkansas to make sense."<<endl;
    } else if (counter_str == "California" && rows < 4){
      if(verbose) cout<<"This is too few rows for California to make sense."<<endl;
    } else if (counter_str == "Nebraska" && (rows < 3 || rows%2==0)){
      if(verbose) cout<<"This is too few rows for Nebraska to make sense, or an even number of rows. Nebraska only works with odd row numbers >= 3"<<endl;
    } else if (counter_str == "Colorado2" && (rows < 3 || rows%2==0)){
      if(verbose) cout<<"This is too few rows for Colorado 2 to make sense, or an even number of rows. Colorado 2 only works with odd row numbers >= 3"<<endl;
    } else if ((counter_str == "Oregon" || counter_str == "Nevada" || counter_str == "NevadaM4" || counter_str == "NevadaM8") && rows%2 == 0){
      if(verbose) cout<<"This is an even number of rows. "<<counter_str<<" only works with odd row numbers"<<endl;
    } else if(counter_str == "NevadaM4" && rows <3) {
      if(verbose) cout<<"Too few rows for modified Nevada"<<endl;
    } else if(counter_str == "NevadaM8" && rows <9) {
      if(verbose) cout<<"Too few rows for modified Nevada - 8"<<endl;
    } else if (counter_str == "Circle_Pent" && circle_var > circle_var_max){
      if(verbose) cout<<"Too far from a centered pentagonal number, circular is not viable"<<endl;
    } else if (counter_str == "Circle_Hex" && circle_var > circle_var_max){
       if(verbose) cout<<"Too far from a centered hexagonal number, circular is not viable"<<endl;
    } else if (counter_str == "Circle_Hex_Flex" && (circle_var > circle_var_max || circle_var%2 == 1)){
       if(verbose) cout<<"Too far from a centered  hexagonal number, circular is not viable. Or remainder is odd, which does not work."<<endl;
    } else if (counter_str == "Diamond" && columns<0){
       if(verbose) cout<<"Diamond not viable."<<endl;
    } else {
      if(verbose)cout<<counter_str<<" is viable with "<<rows<<" x "<<columns<<endl;
      if(flagcsv) fprintf(flagcsv,"%s",counter_str.Data());
      //if(verbose)cout<<command[j]<<endl;
      // fprintf(flagcsv,"\\%s{%i}{%i} \n",command[j],rows,columns);
      numflags++;
      if(verbose) cout<<"Pincer 2.5."<<counter_str<<endl;
      TCanvas* fc;
      bool cantononly;
      if(verbose) cout<<"Pincer 2.5.1"<<counter_str<<endl;
      if (graphics>0){
	if(graphics == 1) {
	  cantononly = kTRUE;
	  fc = new TCanvas("fc","Flag",cantonwidth,cantonheight);
	  fc->Range(0,flagheight-cantonheight,cantonwidth,flagheight); //Changes from NDC coordiantes to pixel coordinates!
	}
	if(verbose) cout<<"Pincer 2.5.2"<<counter_str<<endl;
	if(graphics == 2) {
	  cantononly = kFALSE;
	  fc = new TCanvas("fc","Flag",flagwidth,flagheight);
	  fc->Range(0,0,flagwidth,flagheight); //Changes from NDC coordiantes to pixel coordinates!
	}
	
	//	fc = new TCanvas("fc","Flag",flagwidth,flagheight);
	//	fc->Range(0,0,flagwidth,flagheight); //Changes from NDC coordiantes to pixel coordinates!
	if(verbose) cout<<"Pincer 2.6."<<counter_str<<endl;
	DrawFlag(stripeheight,cantonheight,cantonwidth,flagwidth,flagheight,cantononly);
	if(verbose) cout<<"Pincer 3."<<counter_str<<endl;
	if(counter_str == "Colorado")  { Colorado(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "Oklahoma")  { Oklahoma(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "ColoradoM4")  { ColoradoM4(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "CheckStar")  {CheckStar(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "CheckBlue")  {CheckBlue(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "CheckEven")  {CheckStar(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if( counter_str == "SD")  {SD(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "SD2")  {SD(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "Utah")  {CheckStar(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "CheckEvenInv")  {CheckBlue(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "CheckBorder")  {CheckBorder(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "Grid")  { StarGrid(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "Diamond")  { Diamond(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "Kansas")  { Kansas(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "AL_ME")  { AL_ME(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "Michigan")  { Michigan(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "California")  { California(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "Nebraska")  { Nebraska(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "Colorado2")  { Colorado2(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "Oregon")  { Oregon(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "Nevada")  { Nevada(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "Illinois")  { Illinois(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "Arkansas")  { Arkansas(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "NevadaM4")  { NevadaM4(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "NevadaM8")  { NevadaM8(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "NC")  { NC(rows,columns,cantonwidth,cantonheight,stripeheight);}
	if(counter_str == "Circle_Pent") { Circle(rows,columns,cantonwidth,cantonheight,stripeheight,circle_var);}
	if(counter_str == "Circle_Hex") { Circle(rows,columns,cantonwidth,cantonheight,stripeheight,circle_var);}
	if(counter_str == "Circle_Hex_Flex") { 
	  Circle(rows,columns,cantonwidth,cantonheight,stripeheight,circle_var,chf_pass);
	}
	if(verbose) cout<<"Pincer 4."<<counter_str<<endl;
	TString filename = "./flags/Star";
	if(stars<10) filename+="00";
	if(stars>=10 && stars<100) filename+="0";
	filename+=stars;
	filename+="_";
	filename+=counter_str;
	if(graphics==1) filename+="_canton";
	filename+=".png";
	if(verbose) cout<<"Pincer 5."<<counter_str<<endl;
	fc->SaveAs(filename);
      }
      if(fc != 0){//Prevent memory leak
	delete fc;
	fc=0;
      }

    } 
    //"Colorado","Odd Checkerboard star corner","Odd checkerboard blue corner","Even checkerboard", "Grid", "Kansas"
  }
 
  //fclose(flagcsv);
  if(numflags == 0) cout<<stars<<" Star pattern has NO VIABLE OPTIONS."<<endl;
  if(flagcsv) fprintf(flagcsv, ",%i \n",numflags);

  return numflags;

  

  //The hard ones from Dan Bliss and Slate are: 29(S), 62, 69(S), 79, 87(S) 89
  
  //Improvements
  // danbliss.blogspot.com
  // - reproudce other historical flags
  // - missing flags
  // Made to 1731!
  // - 13-based (13, 14, 19, 20, 46
  // - full border on vertical check star
  // - Memory Leaks
}

void AllStars(int start = 51, int stop = 101, int graphics = 2){
  FILE* flagcsv;
  flagcsv = fopen("AllFlags.csv","w");
  int num;
  cout<<"file is "<<flagcsv<<endl;
  for(int s = start; s<=stop; s++){
    //cout<<s<<" STARS"<<endl;
    num = ViableStyles(s,0,graphics,flagcsv);
    //cout<<endl;
  }
  fclose(flagcsv);
}

void CT(){
  int ring4[3] = {0,18,19};
  int ring5[3] = {0,20,24};
  for(int i = 0; i<=1; i++){
    for(int j = 0; j<=6; j+=6){
      for(int k = 11; k<=13; k++){
	for(int l = 0;l<3;l++){
	  for(int m = 0; m<3; m++){
	    cout<<"Stars = "<<i<<", "<<j<<", "<<k<<", "<<ring4[l]<<", "<<ring5[m]<<","
		<<i+j+k+ring4[l]+ring5[m]<<endl;
	  }
	}
      }
    }
  }
}

