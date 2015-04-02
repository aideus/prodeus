//pointers.h

#ifndef __RAND_EXPR__POINTERS__
#define __RAND_EXPR__POINTERS__

#include <memory>

class Expression;
typedef std::shared_ptr<Expression>       ExpressionP;
typedef std::shared_ptr<const Expression> ExpressionConstP;


class List;
typedef std::shared_ptr<List>       ListP;
typedef std::shared_ptr<const List> ListConstP;

class Cons;
typedef std::shared_ptr<Cons>       ConsP;
typedef std::shared_ptr<const Cons> ConsConstP;

class Symbol;
typedef std::shared_ptr<Symbol>       SymbolP;
typedef std::shared_ptr<const Symbol> SymbolConstP;

class Value;
typedef std::shared_ptr<Value>        ValueP;
typedef std::shared_ptr<const Value>  ValueConstP;

class Lambda;
typedef std::shared_ptr<Lambda>        LambdaP;
typedef std::shared_ptr<const Lambda>  LambdaConstP;

class If;
typedef std::shared_ptr<If>        IfP;
typedef std::shared_ptr<const If>  IfConstP;

class AnnealingQuery_;
typedef std::shared_ptr<AnnealingQuery_>        AnnealingQuery_P;
typedef std::shared_ptr<const AnnealingQuery_>  AnnealingQuery_ConstP;

class GPQuery_;
typedef std::shared_ptr<GPQuery_>        GPQuery_P;
typedef std::shared_ptr<const GPQuery_>  GPQuery_ConstP;

class Define;
typedef std::shared_ptr<Define>        DefineP;
typedef std::shared_ptr<const Define>  DefineConstP;


class Quote_;
typedef std::shared_ptr<Quote_>        Quote_P;
typedef std::shared_ptr<const Quote_>  Quote_ConstP;

class Eval_;
typedef std::shared_ptr<Eval_>        Eval_P;
typedef std::shared_ptr<const Eval_>  Eval_ConstP;

#ifdef SUPPORT_OPENCV
class MatRndInt;
typedef std::shared_ptr<MatRndInt>        MatRndIntP;
typedef std::shared_ptr<const MatRndInt>  MatRndIntConstP;

class MatGaussian;
typedef std::shared_ptr<MatGaussian>        MatGaussianP;
typedef std::shared_ptr<const MatGaussian>  MatGaussianConstP;

#endif


#endif
