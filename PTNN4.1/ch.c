ConstantValue *compileConstant(void)
{
    ConstantValue *constValue;

    switch (lookAhead->tokenType)
    {
    case SB_PLUS:
        eat(SB_PLUS);
        constValue = compileConstant2();
        // constValue->intValue = - constValue->intValue;
        break;
    case SB_MINUS:
        eat(SB_MINUS);
        constValue = compileConstant2();
        constValue->intValue = -constValue->intValue;
        break;
    case TK_CHAR:
        eat(TK_CHAR);
        constValue = makeCharConstant(currentToken->string[0]);
        break;
    default:
        constValue = compileConstant2();
        constValue->intValue = -constValue->intValue;
        break;
    }
    return constValue;
}