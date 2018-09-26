#include<iostream>
#include<vector>
#include<math.h>
#include<stdlib.h>
#include<fstream>
#include<time.h>
using namespace std;


// Global parameters

const int N         = 2;
const int num_steps = 20000000;
int       mcount    = 0;
int       indx      = 2;
bool      one       = true;
double    slope_old = 0.0;
double    Rcomp     = 0.0;
double    Mcomp     = 0.0;
double    lambda    = 0.0;
double    tau       = 0.01;
double    p_end     = 0.0;
double    p_dur     = 0.0;
double    slope     = 0.3;

vector<double>         alpha(4);
vector<double>         masses;
vector<vector<double>> MR_rel;
double*  t;
double** y;
//double y_0[N];
//double y_tau[N];

// Function heads

double line(double p, vector<double> *alpha);

void tov_euler(double *y_t, double t, 
               double *y_t_plus_tau, double tau, vector<double> *alpha);

void f_times_tau(double *y_t, double t, 
                 double *f_times_tau, double tau, vector<double> *alpha);

void star();

// Main function

main(){
 
  int i1, i2;

  t = (double*)malloc((num_steps+1)*sizeof(double));

  if (t == NULL) {
    cout << "Fehler!" << endl; 
    exit(0);
  }

  y = (double**)malloc((num_steps+1)*sizeof(double *));

  if (y == NULL) {
    cout << "Fehler!" << endl; 
    exit(0);
  }

  for (i1 = 0; i1 <= num_steps+1; i1++) {
    y[i1] = (double*)malloc(N*sizeof(double));
    if (y[i1] == NULL) {
      cout << "Fehler!" << endl; 
      exit(0);
    }
  }

  // Variables

//  double y_0[N];
//  double y_tau[N];
  double M, R;
  vector<double> one_MR(2);

  double p_init  = 0.0;

//  double slope   = 0.3;
  double slope_step = -0.01;

  bool   flag_s  = false;
  double diff0_s = 0.0;
  double diff_s  = 0.0;
  bool   flag    = false;
  double diff0   = 0.0;
  double diff    = 0.0;

  double alpha3_old;
  double e_rec;
  double pstep;

//  vector<double> store;
//  vector<double> pao_store;
//  vector<vector<double>> reconstruction;

  vector<double> radii;
  vector<double> diffs;

  double ds;
  double p, e;
  int    indx;
  double eps0, epsm, epsp, epst;

  // File I/O
 
  FILE *MRR = fopen("mr.out", "r");
 
  if (MRR == NULL) {
    exit(0);
  }

  while (1) {
    if (fscanf(MRR, "%lf,%lf", &R, &M) == EOF) {
      break;
    }
    one_MR[0] = R;
    one_MR[1] = M;
    MR_rel.push_back(one_MR);
  }
  fclose(MRR);
  MRR = NULL;
 
  for (i1 = 0; i1 < MR_rel.size(); i1++) {
    if (MR_rel[i1][1] >= 1)
      break;
    mcount++;
  }

  // Initialization

  // p_init  = 0.00001 + mcount * 0.00001;
  p_init   = 4e-5;
  e_rec    = pow(p_init/10., 0.6);
  t[0]     = 0.0000000001;
  p_dur    = p_init;

  alpha[0] = p_init;
  alpha[1] = e_rec;
  alpha[2] = 5 * p_init;
  alpha[3] = e_rec + (alpha[2]-alpha[0]) / slope;

  indx     = 2;
  lambda   = 0;
  // cout << "Slope before mcount loop: " << slope << endl;

  while (mcount < MR_rel.size()) {
    pstep  = 1e-6;
    flag_s = false;
    
    // eps stuff to be added! 

    indx = alpha.size() - 2;

    if (!one) {
      alpha.push_back(alpha[indx] + 1000 * pstep);
      alpha.push_back(alpha[indx+1] + 1000 * pstep / slope);
    }

    indx = alpha.size() - 2; 

    while (slope > 0) {
      slope     += slope_step;
      pstep      = 1e-6;
      alpha3_old = alpha[indx+1];
      // slope = (alpha[indx]-alpha[indx-2]) / (alpha[indx+1]-alpha[indx-1]); 
      alpha[indx+1] = e_rec + (alpha[indx]-alpha[indx-2]) 
                       / (slope + slope_step); 
 
      star();
     // cout << Rcomp << endl;

/*
      flag  = false;
      p_end = p_dur - 5 * pstep;
           
      while (p_end >= 0.8 * p_dur) {
        p_end += pstep; 
        cout << p_end << endl;
	y_0[0] = p_end;
	y_0[1] = 0.0;

        // Initializing
	for (i1 = 0; i1 < N; i1++) {
	  y[0][i1] = y_0[i1];
       	}

	for (i1 = 0; y[i1][0] > 0; i1++) {
          tov_euler(y[i1], t[i1], y_tau, tau, &alpha);
            
          for (i2 = 0; i2 < N; i2++) {
	    y[i1+1][i2] = y_tau[i2];
 	  } 

          t[i1+1] = t[i1] + tau;
        }
        
        cout << "Slope after mass: " << slope << endl; 
        cout << "  P_end for mass: " << p_end << endl;

        if (!flag) {
          diff0 = y[i1-1][1] / 1.4766 - MR_rel[mcount][1];
          // printf("diff0 mass %g %g\n", pstep, diff0);
          flag = true;
          masses.push_back(y[i1-1][1]);
          continue;
        }

        else {
          diff = y[i1-1][1] / 1.4766 - MR_rel[mcount][1];
          printf("diff mass %g %g %g %g\n", pstep, diff, 
                 y[i1-1][1]/1.4766, MR_rel[mcount][1]);
          masses.push_back(y[i1-1][1]);

          if (diff * diff0 > 0) {
            // cout << "diff * diff0 > 0" << endl;
            continue;
          }

          pstep /= 10.0;
          if (pstep < 1e-9) {
            // cout << "pstep < x br. c." << endl;
            break;
          }  vector<double> masses;


          p_end -= 10.0 * pstep;
          continue;
        }

        if (p_end > 5*p_dur) {
          pstep /= -10;
          continue;
        }
      } // end p_end < p_dur loop
*/
// *************************************************************************

      radii.push_back(t[i1-1]);
      masses.clear(); 

      if (!flag_s) {
        diff0_s = t[i1 - 1] - MR_rel[mcount][0];
        // eps0 = pow(diff0_s, 2) + lambda * (slope - slope_old);
        // printf("diff0 radius %f %f %f %f\n", slope_step, diff0_s, t[i1 - 1],
        //        MR_rel[mcount][0]);
        flag_s = true; 

        if (fabs(diff0_s) < 0.005) {
          //cout << "fabs(diff_s)  br. c." << endl;
          break;
        }
        
        diffs.push_back(diff_s);
        continue;
      }
    
     if (!one && slope > 5 /*&& diffs.size() > 100*/ && diff_s >= -0.01) { 
        t[i1-1] += y[i1-1][0] * tau / (y[i1-2][0]-y[i1-1][0]); 
        //       = MR_rel[mcount][0]-diff_s/2;
      }

      diff_s = t[i1 - 1] - MR_rel[mcount][0];
      diffs.push_back(diff_s);
      // cout << "diffs size = " << diffs.size() << endl;
      ds = diffs.size();
/*
      if (!one && slope > 1.5 && diff_s >= -0.01) { 
        t[i1-1] += y[i1-1][0] * tau / (y[i1-2][0]-y[i1-1][0]); 
        //       = MR_rel[mcount][0]-diff_s/2;
      }
*/
      printf("diff radius %f %f %f %f\n", slope_step, diff_s, t[i1 - 1],
             MR_rel[mcount][0]);
      
      if (!one && ds >= 2 && (diffs[ds-1] > 0 && diffs[ds-2] > 0 && 
          diffs[ds-1] > diffs[ds-2]) || (diffs[ds-1] < 0 && diffs[ds-2] < 0 &&
          diffs[ds-1] < diffs[ds-2])) {
        //cout << "Turn around\n";
        slope_step /= -10;
        if (slope_step < 1e-5) 
          slope_step *= 100;
        diffs.clear();
        continue;
      }

      if (!one && diffs.size() > 150) {
        slope += 5*slope_step;
        diffs.clear();
        continue;
      }

      if (fabs(diff_s) <= 0.005) {
        //cout << "fabs(diff_s)  br. c." << endl;
        break;
      }

      if (diff0_s * diff_s > 0) {  
        continue;
      }

      slope_step /= 10;

      if (slope_step < 1e-6) {
        //cout << "slope_step < x br.c." << endl;
        break;
      }

      alpha[indx+1] = alpha3_old;
    } // end slope loop
 
// *************************************************************************

  masses.clear();
  radii.clear();

//  store.push_back(y[i1-1][1]/1.4766);
  mcount++;
   
  e_rec = line(p_end, &alpha);

  cout << "out: " << t[i1-1] << "," << y[i1-1][1]/1.4766 
       << "," << e_rec << "," << p_end
       << "," << MR_rel[mcount-1][0] << "," << MR_rel[mcount-1][1]
       << "," << mcount << "," << slope << endl;

  p_dur = p_end;
  alpha[indx] = p_end;
  alpha[indx+1] = alpha[indx-1] + (alpha[indx]-alpha[indx-2])/slope;
  indx = alpha.size() - 2;

  slope = 0.2;
  slope_step = -0.01;
  diffs.clear();

  one = false;

  } // end mcount loop

// *************************************************************************

  return 0;
}

// Functions

double line(double p, vector<double> *alpha) {
  int i;

  //cout << one << endl;

  if (one || p < (*alpha)[0]) {
    return pow(p/10., 3./5.);
    cout << "known eos\n";
  }

  if (!one) {
    for (i = 2; i < alpha->size(); i += 2) {
      if (p < (*alpha)[i]) { 
        break;
      }
    }
  }

  return (*alpha)[i-1] + (p-(*alpha)[i-2]) * 
         ((*alpha)[i+1]-(*alpha)[i-1])/((*alpha)[i]-(*alpha)[i-2]);
}

void tov_euler(double *y_t, double t, 
               double *y_t_plus_tau, double tau, vector<double> *alpha) {
  int i1;
   
  double k1[N];
  f_times_tau(y_t, t, k1, tau, alpha);

  for (i1 = 0; i1 < N; i1++) {
    y_t_plus_tau[i1] = y_t[i1] + k1[i1];
  }
}

void f_times_tau(double *y_t, double t, 
                 double *f_times_tau, double tau, vector<double> *alpha) {
  f_times_tau[0] = (-(line(y_t[0], alpha) + y_t[0])) *
                   (y_t[1] + 4 * M_PI * pow(t, 3.) * y_t[0]) /
                   (-2*y_t[1] * t + pow(t, 2.)) * tau;
  f_times_tau[1] = tau * 4 * M_PI * pow(t, 2.) * line(y_t[0], alpha);
}

void star() {
  int i1, i2;
  bool flag = false;
  double diffx, diff0, diff = 0.0;
  double pstep = 1e-6;
  double y_tau[2];
  double y_0[2];

  p_end = p_dur - 5*pstep; 

  while (p_end >= 0.8 * p_dur) {
    // loop
    p_end += pstep; 
    y_0[0] = p_end;
    y_0[1] = 0.0;

    // initializing
    for (i1 = 0; i1 < N; i1++) {
        y[0][i1] = y_0[i1];
    }

    // calculation
    for (i1 = 0; y[i1][0] > 0; i1++) {
      tov_euler(y[i1], t[i1], y_tau, tau, &alpha);

      for (i2 = 0; i2 < N; i2++) {
        y[i1+1][i2] = y_tau[i2];
      } 

      t[i1+1] = t[i1] + tau;
    }
    
    // corrections
    //Rcomp = t[i1-1] + y[i1-1][0] * tau / (y[i1-2][0]-y[i1-1][0]);
    //Mcomp = y[i1-1][1] + (y[i1-1][1]-y[i1-2][1])/tau * (Rcomp-t[i1-1]);

    cout << "Slope after mass: " << slope << endl; 
    cout << "  P_end for mass: " << p_end << endl;

    // check0
    if (!flag) {
      diff0 = y[i1-1][1] / 1.4766 - MR_rel[mcount][1];
      // printf("diff0 mass %g %g\n", pstep, diff0);
      if (fabs(diff0) < 1e-6)
        break;
      flag = true;
      // masses.push_back(y[i1-1][1]);
      continue;
    }

    // check
    else {
      diff = y[i1-1][1] / 1.4766 - MR_rel[mcount][1];
      printf("diff mass %g %g %g %g\n", pstep, diff, 
             y[i1-1][1]/1.4766, MR_rel[mcount][1]);
      // masses.push_back(y[i1-1][1]);
   /* ******************************

      if (diff > 0.0) {
        if (fabs(diffx-diff) > 0) {  // > 0??
          diff = diffx;
          break;
        }
      }
      diff = diffx;
      if (fabs(diff) < 1e-6)
        break;

   */// ******************************
      if (diff * diff0 > 0) {
        continue;
      }
      pstep /= 10.0;
      if (pstep < 1e-9) {
        break;
      }
      p_end -= 10.0 * pstep;
      continue;
    }

    if (p_end > 5*p_dur) {
      pstep /= -10;
      continue;
    }  
  } // end p_end < p_dur loop

//  Rcomp = t[i1-1] + y[i1-1][0] * tau / (y[i1-2][0]-y[i1-1][0]);
//  Mcomp = y[i1-1][1] + (y[i1-1][1]-y[i1-2][1])/tau * (Rcomp-t[i1-1]);
//  Mcomp /= 1.4766;
  return;
}








/*
          // will this do the job?
          if (masses.size() > 2) {
            if ((masses[masses.size()-1] - masses[masses.size()-2]) * 
                (masses[masses.size()-2] - masses[masses.size()-3]) < 0) {
              
 
              // de-softener / stiffener
              // further adjust factor... 
              slope += 0.41 * slope_step;

              masses.clear();

              if (slope > 1.5) 
                break;

              // cout << "test  " 
              // << fabs(MR_rel[mcount][1] - masses[masses.size()-2]) << endl;
  
              if (fabs(diff) < pow(10., -5.)) {
                break; 
              }

              // slope = (alpha[2]-alpha[0]) / (alpha[3]-e_rec); 
              alpha[indx+1] = e_rec + (alpha[indx]-alpha[indx-2]) / (slope); 

              continue;
            }
          }
*/

