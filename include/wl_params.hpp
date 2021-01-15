#ifndef WANGLANDAU_WL_PARAMS_H_
#define WANGLANDAU_WL_PARAMS_H_


#include <iostream>


class WLParams {
 public:
  WLParams(int check_flatness_every, double lnf, double lnfmin, double flatness,
      double overlap, int swap_every) :
    check_flatness_every_(check_flatness_every),
    lnf_(lnf),
    lnfmin_(lnfmin),
    flatness_(flatness),
    overlap_(overlap),
    swap_every_(swap_every) {}
  void update_lnf() {lnf_ /= 2.0;}
  void set_lnf(double lnf) {lnf_ = lnf;}
  // Gettor.
  int check_flatness_every() const {return check_flatness_every_;}
  double lnf() const {return lnf_;}
  double lnfmin() const {return lnfmin_;}
  double flatness() const {return flatness_;}
  double overlap() const {return overlap_;}
  int swap_every() const {return swap_every_;}
 private:
  int check_flatness_every_, swap_every_;
  double lnf_, lnfmin_, flatness_, overlap_;
};


#endif // WANGLANDAU_WL_PARAMS_H_