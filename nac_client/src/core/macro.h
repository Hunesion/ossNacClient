#ifndef _HUNE_CORE_MACRO_H_
#define _HUNE_CORE_MACRO_H_

#define BEGIN_HUNE \
    namespace Hune \
    {
#define END_HUNE }
#define BEGIN_HUNE_CORE \
    namespace Hune      \
    {                  \
    namespace Core     \
    {
#define END_HUNE_CORE \
    }                \
    }

#define USING_HUNE using namespace Hune
#define USING_HUNE_CORE using namespace Hune::Core


#define HUNE_SYNTHESIZE(varType, varName, funName)\
protected: varType varName; public: virtual inline varType get##funName() const { return varName; } virtual inline void set##funName(varType var){ varName = var; }

#define HUNE_SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName; public: virtual inline varType& get##funName() { return varName; } virtual inline void set##funName(const varType& var){ varName = var; }

#define HUNE_SYNTHESIZE_READONLY(varType, varName, funName)\
protected: varType varName; public: virtual inline varType get##funName() const { return varName; }

#define HUNE_SYNTHESIZE_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName; public: virtual inline const varType& get##funName() const { return varName; }



#define HUNE_CALLBACK_OBJECT_0(__func__, __target__, ...) std::bind(&__func__, __target__, ##__VA_ARGS__)
#define HUNE_CALLBACK_OBJECT_1(__func__, __target__, ...) std::bind(&__func__, __target__, std::placeholders::_1, ##__VA_ARGS__)
#define HUNE_CALLBACK_OBJECT_2(__func__, __target__, ...) std::bind(&__func__, __target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)

#define HUNE_CALLBACK_CLS_STATIC_0(__func__, ...) std::bind(&__func__, ##__VA_ARGS__)
#define HUNE_CALLBACK_CLS_STATIC_1(__func__, ...) std::bind(&__func__, std::placeholders::_1, ##__VA_ARGS__)
#define HUNE_CALLBACK_CLS_STATIC_2(__func__, ...) std::bind(&__func__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)



#endif /* _HUNE_CORE_MACRO_H_ */