 //  Double_t x[5] = {.2,.7,.6,.25,.2};
  // Double_t y[5] = {.5,.1,.9,.7,.5};
  // TPolyLine *pline = new TPolyLine(5,x,y);
  // pline->SetFillColor(38);
  // pline->SetLineColor(2);
  // pline->SetLineWidth(4);
  // pline->Draw("f");
   // pline->Draw();

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

  //TColor* OldGloryBlue = gROOT->GetColor(1756);
  //TColor* OldGloryBlue = new TColor(1756,0.234,0.233,0.430,"OGB");
  if(verbose) cout<<"DF Pincer 0.3."<<OldGloryBlue<<endl;
  // OldGloryBlue->SetRGB(0.234,0.233,0.430);
  //cout<<"DF Pincer 0.4"<<endl;
  //TColor* OldGloryRed =  gROOT->GetColor(1757);
  // TColor* OldGloryRed = new TColor(1757,0.698,0.132,0.203,"OGR");
  //cout<<"DF Pincer 0.5"<<endl;
  //OldGloryRed->SetRGB(.698,0.132,0.203);
  //cout<<"DF Pincer 1.0"<<endl;
  
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
  

  //int rows = 6;
  //int columns = 8;
  //double cw = cantonwidth;
  //double ch = cantonheight;
  //double stripe = stripeheight;

  /*
  //---------
  double star_space_width = cw/(columns + 1);
  double star_space_height = ch/(rows + 1);
  double star_diameter = stripeheight*0.8;

  double x1 = 0;
  double y1 = 1.0;
  
  for(int r = 1; r<=rows; r++){
    for(int c = 1; c<=columns; c++){
      DrawStar(c*star_space_width + x1,y1 - r*star_space_height,star_diameter);
    }
  }
  //---
  */

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

  
  
  

  //double interstar_space_height = star_space_height;
  
  

  //canton_border_height = 0;
  //canton_border_width = 0;
  /*
 //Border Activation
  if(canton_border_height < minimum_canton_border_height){
    cout<<"Height = "<<canton_border_height<<", Minimum = "<<minimum_canton_border_height<<endl;
    canton_border_height = minimum_canton_border_height;
    star_space_height = (ch-2*canton_border_height)/(rows + 1);
    cout<<"HA - Check"<<endl;
  } else {
    canton_border_height = 0;
  }
 */
  
  
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

  /*

  if(canton_border_width < minimum_canton_border_width){
    cout<<"WA - Check -- Minimum: "<<minimum_canton_border_width<<", Actual = "<<canton_border_width<<endl;
    canton_border_width = minimum_canton_border_width;
    star_space_width = (cw-2*canton_border_width)/(columns);
  } else {
    canton_border_width = 0;
  }

  //No check seems to be needed for minimum height
  
  if(canton_border_height > maximum_canton_border_height){
    cout<<"HM - Check: "<<maximum_canton_border_height<<", Actual = "<<canton_border_height<<endl;
    cout<<"Star Spacing Height = "<< star_space_height<<endl;
    canton_border_height = maximum_canton_border_height;
    star_space_height = (ch-2*canton_border_height)/(rows);
    cout<<"After Star Spacing Height = "<< star_space_height<<endl;
  } else {
    canton_border_height = 0;
  }

  */

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

/*
void CheckBlue(int rows, int columns, double cw = 0.4, double ch = 7.0/13.0, double stripe=1/13.0){
  double star_space_width = cw/(columns + 1);
  double star_space_height = ch/(rows + 1);
  double star_diameter = stripe*0.8;

  double x1 = 0;
  double y1 = 1.0;
  
  for(int r = 1; r<=rows; r++){
    for(int c = 1; c<=columns; c++){
      if( (r%2 ==1 && c%2 == 0) || (r%2 ==0 && c%2 ==1))
	DrawStar(c*star_space_width + x1,y1 - r*star_space_height,star_diameter);
    }
  }
}
*/

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


