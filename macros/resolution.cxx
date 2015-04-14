{

  TF1* ecalEB = new TF1("ecalEB","1/x*sqrt(x^2*0.007^2 + x*0.029^2 + 1.01^2)",0.1,500);
  TF1* ecalEE = new TF1("ecalEE","1/x*sqrt(x^2*0.02^2 + x*0.087^2 + 1.95^2)",0.1,500);
  TF1* ecalHF = new TF1("ecalHF","1/x*sqrt(x^2*0.29^2 + x*0.86^2 + 191^2)",0.1,500);

  ecalEB->SetLineWidth(2);
  ecalEE->SetLineWidth(2);
  ecalHF->SetLineWidth(2);

  ecalEB->SetLineColor(kBlack);
  ecalEE->SetLineColor(kRed);
  ecalHF->SetLineColor(kBlue);

  TCanvas* c1 = new TCanvas("c1","",500,500);
  c1->cd();
  c1->SetLogy();

  ecalEE->Draw();
  ecalEB->Draw("same");
  ecalHF->Draw("same");


  TF1* hcalEB = new TF1("hcalEB","1/x*(x*0.132 - sqrt(x)*0.285 + 10)",0.1,500);
  TF1* hcalEE = new TF1("hcalEE","1/x*(x*0.0737 - sqrt(x)*0.0343 + 7.3)",0.1,500);
  TF1* hcalHF = new TF1("hcalHF","1/x*(x*0.239 + sqrt(x)*1.95 + 19.1)",0.1,500);
  TF1* hcalHF2 = new TF1("hcalHF","1/x*(x*0.0732 + sqrt(x)*14.7+42.8)",0.1,500);

  hcalEB->SetLineWidth(2);
  hcalEE->SetLineWidth(2);
  hcalHF->SetLineWidth(2);
  hcalHF2->SetLineWidth(2);

  hcalEB->SetLineColor(kBlack);
  hcalEE->SetLineColor(kRed);
  hcalHF->SetLineColor(kBlue);
  hcalHF2->SetLineColor(kGreen+1);

  TCanvas* c2 = new TCanvas("c2","",500,500);
  c2->cd();
  //  c2->DrawFrame(0.1,0,500,100);

  hcalEB->Draw();
  hcalEE->Draw("same");
  hcalHF->Draw("same");
  hcalHF2->Draw("same");




}
