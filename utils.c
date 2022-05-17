/** this file contains helper functions */

int16_t addWithRangeCheck(int16_t value, int16_t summand, int16_t lower, int16_t upper){
  if ( (value >= lower) & (value <= upper) ){
    if ( value + summand > upper ){
      return upper;
    }
    else if ( value + summand < 0){
      return lower;
    }
    else {
      return value += summand;
    }
  }
}
