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

#include "TPolyLine.h"
#include "TMath.h"
#include "TColor.h"
#include "TROOT.h"

void DrawFlag(double stripeheight, double cantonheight, double cantonwidth,
	      double flagwidth, double flagheight,bool cantononly){
  bool verbose=kFALSE;


  if(verbose) cout<<"canton height = "<<cantonheight<<" and stripe height = "<<stripeheight<<endl;
  TColor* OldGloryBlue;
  TColor* OldGloryRed;

  OldGloryBlue = gROOT->GetColor(1756);
  OldGloryRed =  gROOT->GetColor(1757);

  Float_t OGB_rgb[3] = {0.234,0.233,0.430};
  Float_t OGR_rgb[3] = {0.698,0.132,0.203};
  if(verbose) cout<<"Pincer 1.0"<<endl;

  if(! OldGloryBlue)  {
    if(verbose) cout<<"Pincer 1.1"<<endl;
    OldGloryBlue = new TColor(1756,OGB_rgb[0],OGB_rgb[1],OGB_rgb[2],"OGB");
  }

  if( (OldGloryBlue->GetRed() != OGB_rgb[0] &&
       OldGloryBlue->GetGreen() != OGB_rgb[1] &&
       OldGloryBlue->GetBlue() != OGB_rgb[2])){ //if the color has wrong values
    if(verbose) cout<<"Pincer 1.2"<<endl;
    OldGloryBlue->SetRGB(OGB_rgb[0],OGB_rgb[1],OGB_rgb[2]); // set it
  }

  if(verbose) cout<<"Pincer 2.0"<<endl;

  if(! OldGloryRed)  OldGloryRed = new TColor(1757,OGR_rgb[0],OGR_rgb[1],OGR_rgb[2],"OGR");

  if (OldGloryRed->GetRed() != OGR_rgb[0] &&
      OldGloryRed->GetGreen() != OGR_rgb[1] &&
      OldGloryRed->GetBlue() != OGR_rgb[2]){ //if the color has wrong values
    OldGloryRed->SetRGB(OGR_rgb[0],OGR_rgb[1],OGR_rgb[2]); // set it
  }


  if(verbose) cout<<"DF Pincer 0.3."<<OldGloryBlue<<endl;

  TList* stripelist = new TList();
  if(verbose) cout<<"DF Pincer 1.1"<<endl;
  TBox stripe[13];

  if(verbose) cout<<"Make Stripes"<<endl;
  //Make stripes
  for(int i = 0; i<13; i=i+2)  stripelist->Add(new TBox(0,i*stripeheight,flagwidth,(i+1)*stripeheight));

  //Draw stripes
  TIterator* stripeiter = stripelist->MakeIterator();
  TBox* tempstripe = new TBox();
  if(!cantononly){
    while((tempstripe = (TBox*)stripeiter->Next())){
      tempstripe->SetFillColor(1757);
      tempstripe->Draw();
    }
  }

 if(verbose)  cout<<"Make Canton"<<endl;
  //Make Canton
  TBox* canton = new TBox(0,flagheight,cantonwidth,flagheight-cantonheight);
  canton->SetFillColor(1756);
  canton->Draw();

  // StarGrid(6,8,cantonwidth,cantonheight,stripeheight);




}

void DrawStar(double x, double y, double dia){

 double theta_step = (360.0/5.0)*(TMath::Pi()/180.0);
  double theta_mod =0;
  double inner_theta_mod = 0;
  double theta = 0;
  double out_r = dia/2.0;

  double star_points = 5;
  double star_angle = (2*TMath::Pi())/star_points;
  double radius_ratio = (TMath::Sin(star_angle/4))/(TMath::Sin(TMath::Pi() - (3*star_angle/4)));

  //double in_r = out_r/2.5;
  double in_r = out_r*radius_ratio;

  double x2[11];
  double y2[11];

  //theta_step = (360.0/10.0)*(TMath::Pi()/180.0);

  for(int i = 0; i<6; i++){
    theta = i*theta_step;
    theta_mod = theta + TMath::Pi()/2;
    inner_theta_mod = theta_mod + theta_step/2.0;
    x2[2*i] = (out_r)*TMath::Cos(theta_mod) + x;
    y2[2*i] = out_r*TMath::Sin(theta_mod) + y;

    // cout<<"x2["<<i<<"] = "<<x2[i]<<" from Cos("<<theta_mod<<")"<<" = "<<TMath::Cos(theta_mod)<<endl;

    if(2*i+1<=10){
      x2[2*i+1] = (in_r)*TMath::Cos(inner_theta_mod) + x;
      y2[2*i+1] = in_r*TMath::Sin(inner_theta_mod) + y;
    }
  }

  // for(int j = 0; j<11; j++) cout<<"x = "<<x2[j]<<", y = "<<y2[j]<<endl;

  TPolyLine* star_2 = new TPolyLine(11,x2,y2);
  star_2->SetFillColor(kWhite);
  star_2->SetLineColor(kBlack);
  //star_2->Draw();
  star_2->Draw("f");

}


//-----------CANTON STYLES ------------
//double stripeheight = 1/13.0, double cantonheight = 7.0/13.0, double cantonwidth = 0.4

//--Checkerboard Styles--
void CheckStar(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0, TString star="star"){
  double star_space_width = cw/(columns + 1);
  double star_space_height = ch/(rows + 1);
  double star_diameter = stripe*0.8;
  //cout<<"Star Diameter = "<< star_diameter<<endl;
  double flag_ratio = 1.9;

  int stars_drawn = 0;

  int starpoints = 5;
  double interior_star_angle = (TMath::Pi()*2)/starpoints;

  double x1 = 0;
  double y1 = stripe*13;

  bool condition;







  double interstar_space_width =  star_space_width*2;
  if(star=="border") interstar_space_width =  star_space_width;
  double crossbar_length = (star_diameter/2)*TMath::Sin(interior_star_angle)*2;

  //Here border means distance from edge of canton to edge of nearest star circle
  double minimum_canton_border_height = ch/10 - (star_diameter/2);//height
  double minimum_canton_border_width = cw/18 - (star_diameter/2);//

  if(interstar_space_width < crossbar_length){
    cout<<"W - Check - Need to shrink stars"<<endl;
    star_diameter = interstar_space_width*0.9;
  }

  if(star_space_height < (star_diameter/2)*(10./9.)){
    cout<<"Height - Check - Need to shrink stars"<<endl;
    star_diameter = (star_space_height*2)*0.8;
  }



  double maximum_canton_border_height = ch*(0.056/(7./13.)) - (star_diameter/2);//height
  double maximum_canton_border_width = cw/12 - (star_diameter/2);

  double canton_border_width = star_space_width - (star_diameter/2);
  double canton_border_height = star_space_height - (star_diameter/2);
  cout<<"Width = "<<canton_border_width<<", Minimum = "<<minimum_canton_border_width<<endl;


  for(int r = 1; r<=rows; r++){
    for(int c = 1; c<=columns; c++){
      condition =  (r%2 ==0 && c%2 ==0) || (r%2 ==1 && c%2 ==1); //defualt style is "star")
      if(star=="blue") condition = (r%2 ==1 && c%2 == 0) || (r%2 ==0 && c%2 ==1);
      if(star=="border") condition = ((c==1 || c==columns) && r%2==1 )||
			   ((c>1 && c<columns) && ( (r%2 ==1 && c%2 == 0) || (r%2 ==0 && c%2 ==1) ));
      if(star == "SD") condition =  ((r%2 ==0 && c%2 ==0) || (r%2 ==1 && c%2 ==1)) && !(r>1 && c==1);
      if(condition){
	//	cout<<"Star x = "<<c*star_space_width + x1<<", Star y = "<<y1 - r*star_space_height<<endl;
	DrawStar(c*star_space_width + x1//+ canton_border_width
		 ,
		 y1 - r*star_space_height// - canton_border_height
		 ,
		 star_diameter);
	stars_drawn++;
      }
    }
  }
  cout<<"Stars Drawn = "<<stars_drawn<<endl;
}

void CheckBlue(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  CheckStar( rows,  columns,  cw ,  ch ,  stripe, "blue");
}

void CheckBorder(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  CheckStar( rows,  columns,  cw ,  ch ,  stripe, "border");
}

void SD(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  CheckStar( rows,  columns+1,  cw ,  ch ,  stripe, "SD");
}



//Grid Styles

void StarGrid(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0,
	      TString style = "Grid", double star_diameter_override = 0){
  //ch is fraction of flag height (NDC coordinates)
  //cw is fraction of flag width (NDC coordinates)
  int diamond_extra = 0;
  int diamond_extra_rows = diamond_extra/4; //This only works when the extra is a multiple of 4
  if(style == "Diamond") {
    diamond_extra = columns;
    diamond_extra_rows = diamond_extra/4;
    cout<<"Extra rows = "<<diamond_extra_rows<<endl;
    columns = rows;//Diamond only works with square grid as basis
  }
  int stars_drawn = 0;

  cout<<"In Grid for style "<<style<<"."<<endl;

  double star_space_width = cw/(columns);
  double star_space_height = ch/(rows);
  double star_diameter = stripe*0.8;
  if(star_diameter_override) {
    star_diameter = star_diameter_override;
    cout<<"Star Diameter Overridden to "<<star_diameter<<endl;
  }
  double star_radius = star_diameter/2.0;

  double canton_border_width = 0.5*star_space_width;//https://en.wikisource.org/wiki/Executive_Order_1637
  double canton_border_height = 0.5*star_space_height;


  cout<<"Grid rows = "<<rows<<" x columns = "<<columns<<endl;
  //"border" == distance from edge of canton to center of nearest star.

  double x1 = 0;
  double y1 = stripe*13;

  Bool_t condition1 = kTRUE;
  Bool_t condition2 = kFALSE;
  double extra_space_width = 0;

  int midrow = ((rows + 1)/2) - 1;//assume number of rows is odd!
  int midcolumn = ((columns + 1)/2) - 1;

  //int mr1 = 0;
  if(rows%2 == 0){//if we have an even number of rows
    midrow = (rows/2)-1; //Subtract 1 to account for counting from zero
  }


  cout<<"Middle rows = "<<midrow<<"; middle column = "<<midcolumn<<endl;

  double star_points = 5;
  double star_angle = (2*TMath::Pi())/star_points;
  double radius_ratio = (TMath::Sin(star_angle/4))/(TMath::Sin(TMath::Pi() - (3*star_angle/4)));
  double interior_star_angle = (TMath::Pi()*2)/star_points;


  //double in_r = out_r/2.5;
  double in_r = star_radius*radius_ratio;





  //cout<<"Style = "<<style<<". Rows = "<<rows<<", Columns = "<<columns<<endl;
  //cout<<"Star Space = "<<star_space_height<<". Clearance = "<<star_radius + in_r<<endl;
  if(star_space_height < star_radius + in_r*2){
    cout<<"Grid - H - Shrinking Stars"<<endl;
    star_diameter = star_space_height;
  }

  double crossbar_length = (star_diameter/2)*TMath::Sin(interior_star_angle)*2;

  if(star_space_width < crossbar_length){
    cout<<"Grid - W - Shrinking Stars"<<endl;
    star_diameter = star_space_width*0.9;
  }

  double minimum_canton_border_height = star_diameter*0.726;//height
  double minimum_canton_border_width = star_diameter*0.772;



  //Border Shrinking Criteria
  if(canton_border_height < minimum_canton_border_height){
    //For 48-star, star_diameter = flag_height*(1/13)*(4/5)
    //For 48-star, canton_border = (flag_height*(7/13))/12
    // flag_height = canton_border*12*(13/7);
    // star_diameter = canton_border*12*(1/7)*(4/5)
    //               = canton_border*(48/35)
    // cout<<"Crowding Stars"<<endl;
    //cout<<"Star radius = "<< star_radius<<endl;
    // star_diameter = canton_border_height*(48.0/35.0);
    // star_space_width = cw/(columns+1);
    // star_space_height = ch/(rows+1);
    //canton_border_width = star_space_width;
    //canton_border_height = star_space_height;
    canton_border_height = minimum_canton_border_height;
    star_space_height = (ch-2*canton_border_height)/(rows-1);
    cout<<"HA - Grid"<<endl;
  }

  if(canton_border_width < minimum_canton_border_width){
    canton_border_width = minimum_canton_border_width;
    star_space_width = (cw-2*canton_border_width)/(columns-1);
    cout<<"WA - Grid"<<endl;
  }

  if(style == "Pillars"){
    columns = 2;
    canton_border_width = minimum_canton_border_width;
    star_space_width = cw -2*canton_border_width;
    if(rows == 1) {
      canton_border_height = ch/2;
    } else {
      canton_border_height = minimum_canton_border_height;
      star_space_height =  (ch-2*canton_border_height)/(rows-1);
    }

  }


  for(int r = 0; r<rows; r++){
    for(int c = 0; c<columns; c++){
      if(style == "Colorado") {
	condition1 = (r!=0 && r!=(rows-1));
	condition2 = (c<columns -1);
	extra_space_width = 0.5*star_space_width;
      }
      if(style == "Oklahoma") {
	condition1 = (r!=1 && r!=(rows-2));
	condition2 = (c<columns -1);
	extra_space_width = 0.5*star_space_width;
      }
      if(style == "ColoradoM4") {
	condition1 = (r!=0 && r!=(rows-1));
	condition2 = (c<columns -3);
	extra_space_width = 1.5*star_space_width;
      }
      if(style == "NC") {
	condition1 = (r!=0 && r!=(rows-1));
	condition2 = (c<columns -2);
	extra_space_width = star_space_width;
      }
      if(style == "Kansas") {
	condition1 = (r!=midrow);
	condition2 = (c<columns -1);
	extra_space_width = 0.5*star_space_width;
      }
      if(style == "AL_ME") {
	condition1 = (r!=midrow);
	condition2 = (c<columns -1);
	extra_space_width = 0.5*star_space_width;
      }
      if(style == "Michigan"){
	condition1 = (r==0 || r==(rows-1));
	condition2 = (c<columns -1);
	extra_space_width = 0.5*star_space_width;
      }
      if(style == "California"){
	condition1 = (r==0 || r==(rows-1) || (r==1 && c!=0 and c!=columns-1));
	condition2 = (r!=0 && r!=1 && r!= (rows-1) && c<columns -1);
	extra_space_width = 0.5*star_space_width;
      }
      if(style == "Colorado2"){
	condition1 = (r%2==0);//Zeroth and other Even rows (counting from zero)
	condition2 = (r%2==1 && c<columns-1);
	extra_space_width = 0;
      }
      if(style == "Nebraska"){
	condition1 = (r%2==1);//Second and other odd rows (counting from zero)
	condition2 = (r%2==0 && c>0);
	extra_space_width = 0;
      }
      if(style == "Oregon"){
	condition1 = (r!=midrow);
	condition2 = (c<columns -2);
	extra_space_width = star_space_width;
      }
      if(style == "Nevada"){
	condition1 = (r%2==0);//First and other even rows (counting from zero)
	condition2 = (c<columns -2);
	extra_space_width = star_space_width;
      }
      if(style == "Illinois"){
	condition1 = ( (r==1 && c!=0 && c!=columns -1) || r>1 ); //Second and following rows (counting from zero)
	condition2 = (r==0 && c!=columns-1); //First row (counting from zero)
	extra_space_width = star_space_width/2;
      }
      if(style == "Arkansas"){
	condition1 = (r>=2); //Third and following rows (counting from zero)
	condition2 =0;
      }
      if(style == "Arkansas_top2"){
	condition1 = (r==0); //First Row (counting from zero)
	condition2 = (r == 1 && c<columns - 1);
	extra_space_width = star_space_width/2;
      }
      if(style == "NevadaM4"){
	condition1 = (r%2==0 && r!=0 && r!=rows-1);//Even rows (counting from zero), except first and last
	condition2 = (c<columns -2);
	extra_space_width = star_space_width;
      }
      if(style == "NevadaM8"){
	condition1 = (r%2==0 && r!=0 && r!=2 && r!=rows-1 && r!=rows-3);//Even rows (counting from zero), except first and last 2
	condition2 = (c<columns -2);
	extra_space_width = star_space_width;
      }
      if(style == "Diamond"){
	condition1 = (r<=midrow && c >= midcolumn-r && c <= midcolumn+r) ||
	  (r>midrow && c >= midcolumn - (rows-(r+1)) && c <= midcolumn + (rows-(r+1)) );
	condition2 = (diamond_extra_rows>1 && r!=midrow && TMath::Abs(r-midrow)<=diamond_extra_rows &&(c==0 || c==columns-1))
	  ||(diamond_extra_rows == 1 && (r == 0 || r == rows -1)  && (c == 0 || c == columns -1));
	//TMath::Abs(r-midrow)
      }

      if(condition1){
	DrawStar(c*star_space_width + x1 + canton_border_width,
		 y1 - r*star_space_height - canton_border_height,
		 star_diameter);
	stars_drawn++;
      } else if(condition2){
	DrawStar(c*star_space_width + x1 + canton_border_width + extra_space_width,
		 y1 - r*star_space_height - canton_border_height,
		 star_diameter);
	stars_drawn++;
      }
    }
  }
  cout<<"Grid Stars Drawn = "<<stars_drawn<<endl;
}

void Colorado(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  StarGrid(rows,columns,cw,ch,stripe,"Colorado");
}

void Oklahoma(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  StarGrid(rows,columns,cw,ch,stripe,"Oklahoma");
}

void ColoradoM4(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  StarGrid(rows,columns,cw,ch,stripe,"ColoradoM4");
}

void NC(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  StarGrid(rows,columns,cw,ch,stripe,"NC");
}

void Kansas(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  StarGrid(rows,columns,cw,ch,stripe,"Kansas");
}

void AL_ME(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  StarGrid(rows,columns,cw,ch,stripe,"AL_ME");
}

void Michigan(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  StarGrid(rows,columns+1,cw,ch,stripe,"Michigan");
}

void California(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  StarGrid(rows,columns+1,cw,ch,stripe,"California");
}

void Nebraska(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  StarGrid(rows,columns,cw,ch,stripe,"Nebraska");
}

void Colorado2(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  StarGrid(rows,columns,cw,ch,stripe,"Colorado2");
}

void Nevada(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  StarGrid(rows,columns+1,cw,ch,stripe,"Nevada");
}

void Illinois(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  StarGrid(rows,columns,cw,ch,stripe,"Illinois");
}

void Arkansas(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  StarGrid(rows,columns-1,cw,ch,stripe,"Arkansas_top2");
  StarGrid(rows,columns,cw,ch,stripe,"Arkansas");
}

void NevadaM4(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  StarGrid(rows,columns+1,cw,ch,stripe,"NevadaM4");
}
void NevadaM8(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  StarGrid(rows,columns+1,cw,ch,stripe,"NevadaM8");
}

void Oregon(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  StarGrid(rows,columns,cw,ch,stripe,"Oregon");
}

void Diamond(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  StarGrid(rows,columns,cw,ch,stripe,"Diamond");
}

void Circle(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0,
	    int remainder = 0, int* chf_pass = NULL){
  //columns = 5 or 6 (pent or hex)
  //rows = number of rings outside of the central star
  double star_diameter = stripe*0.8;
  double x1 = cw/2.0;
  double y1 = stripe*13 - ch/2.0;

  double star_angle = (2*TMath::Pi())/5;
  double radius_ratio = (TMath::Sin(star_angle/4))/(TMath::Sin(TMath::Pi() - (3*star_angle/4)));

  //double in_r = out_r/2.5;
  double in_r = (star_diameter/2)*radius_ratio;

  double star_r = 0;
  double star_theta = 0;
  double theta_inc = 0;
  double theta_start = TMath::Pi()/2;
  double max_r = ch/2 - star_diameter*0.726; //Same minimum as grid
  double r_inc = max_r/rows;
  bool star_shrink = kFALSE;
  cout<<"Circle "<<columns<<" increment = "<<r_inc<<endl;

  double r_inc_min =  0.9*star_diameter;
  if(columns == 6) r_inc_min = star_diameter;

  if(r_inc < r_inc_min) {// 44.9108 too small, 56.1385 is OK, star diameter is 61.5385
    cout<<"Circle, srinking stars from "<<star_diameter<<" to "<<r_inc<<endl;
    star_diameter = r_inc;
    star_shrink = 1;
  }

  int stars_in_ring = 0;//1,5,10,15

  cout<<"Columns = "<<columns<<", Rows = "<<rows<<", Remainder = "<<remainder<<endl;

  if(remainder != 0 && remainder != -1){
    if(remainder%2 == 0) {
      if(star_shrink) {
	cout<<"Star Shrunk!"<<endl;
	StarGrid(remainder/2,2,cw,ch,stripe,"Pillars",star_diameter);
      } else {
	StarGrid(remainder/2,2,cw,ch,stripe,"Pillars",0);
      }
    }
    if(remainder%2 == 1 && !(remainder == 5 && rows == 3)) {//5 is a special case (see below)
      cout<<"Adding "<<(remainder+1)/2<<" stars in pillars"<<endl;
      if(star_shrink) {
	cout<<"Star Shrunk!"<<endl;
	StarGrid((remainder+1)/2,2,cw,ch,stripe,"Pillars",star_diameter);
      } else {
	StarGrid((remainder+1)/2,2,cw,ch,stripe,"Pillars",0);
      }
      remainder = -1;
    }
  }

  int i = 0;
  if(remainder == -1) i = 1;
  for(i; i<=rows; i++){//i = star ring
    if(chf_pass){
      stars_in_ring = chf_pass[i];
    } else if(!chf_pass && i==0){
      stars_in_ring = 1;
    } else {
      stars_in_ring = columns*i;
      if(remainder == 5 && rows == 3 && i == 3) stars_in_ring = columns*(i+1);
    }
    star_r = r_inc*i;
    theta_inc = (2*TMath::Pi())/stars_in_ring;
    for(int j = 0; j<stars_in_ring; j++){
      star_theta = j*theta_inc + theta_start;
      DrawStar(x1 + star_r*TMath::Cos(star_theta),y1 + star_r*TMath::Sin(star_theta),star_diameter);
    }
  }


}




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

