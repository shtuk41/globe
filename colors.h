struct RGB
{
    RGB()
    {
      R = G = B = 0;
    }

    void Set(double r, double g, double b)
    {
      R = r;
      G = g;
      B = b;
    }

    double   R;
    double   G;
    double   B;

};


/*
  H - 0 - 360
  S - 0 - 1
  V - 0 - 1
*/
RGB ConvertHSVtoRGB(double H, double S, double V)
{
  double C = V * S;
  double HH = H / 60;
  double X = C * (1.0 - fabs(fmod(HH, 2) - 1.0));

  RGB rgb_temp;

  if (HH >= 0 && HH <= 1)
  {
      rgb_temp.Set(C,X,0);
  }
  else if (HH >= 1 && HH <= 2)
  {
      rgb_temp.Set(X,C,0);
  }
  else if (HH >= 2 && HH <= 3)
  {
      rgb_temp.Set(0,C,X);
  }
  else if (HH >= 3 && HH <= 4)
  {
      rgb_temp.Set(0,X,C);
  } 
  else if (HH >= 4 && HH <= 5)
  {
      rgb_temp.Set(X,0,C);
  }
  else if (HH >= 5 && HH < 6)
  {
      rgb_temp.Set(C,0,X);
  }

  double m = V - C;

  RGB rgb;

  rgb.Set(rgb_temp.R + m, rgb_temp.G + m, rgb_temp.B + m);

  return rgb;
}