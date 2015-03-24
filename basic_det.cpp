//
//  basic_det.cpp
//  rand_expr
//
//  Created by Alexey Potapov on 01/02/15.
//  Copyright (c) 2015 Alexey Potapov. All rights reserved.
//

#include "basic_det.h"

Cos_ Cos(const Expression &e) { return Cos_(&e); }
Sin_ Sin(const Expression &e) { return Sin_(&e); }
Tan_ Tan(const Expression &e) { return Tan_(&e); }
Log_ Log(const Expression &e) { return Log_(&e); }
Sqrt_ Sqrt(const Expression &e) { return Sqrt_(&e); }
Exp_ Exp(const Expression &e) { return Exp_(&e); }
Floor_ Floor(const Expression &e) { return Floor_(&e); }
Ceil_ Ceil(const Expression &e) { return Ceil_(&e); }
