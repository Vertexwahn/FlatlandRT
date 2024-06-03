// Copyright 2018 The Abseil Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "absl/debugging/internal/demangle.h"

#include <cstdlib>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "absl/base/config.h"
#include "absl/debugging/internal/stack_consumption.h"
#include "absl/log/log.h"
#include "absl/memory/memory.h"

namespace absl {
ABSL_NAMESPACE_BEGIN
namespace debugging_internal {
namespace {

using ::testing::ContainsRegex;

TEST(Demangle, FunctionTemplate) {
  char tmp[100];

  // template <typename T>
  // int foo(T);
  //
  // foo<int>(5);
  ASSERT_TRUE(Demangle("_Z3fooIiEiT_", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "foo<>()");
}

TEST(Demangle, FunctionTemplateWithNesting) {
  char tmp[100];

  // template <typename T>
  // int foo(T);
  //
  // foo<Wrapper<int>>({ .value = 5 });
  ASSERT_TRUE(Demangle("_Z3fooI7WrapperIiEEiT_", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "foo<>()");
}

TEST(Demangle, FunctionTemplateWithNonTypeParamConstraint) {
  char tmp[100];

  // template <std::integral T>
  // int foo(T);
  //
  // foo<int>(5);
  ASSERT_TRUE(Demangle("_Z3fooITkSt8integraliEiT_", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "foo<>()");
}

TEST(Demangle, FunctionTemplateWithFunctionRequiresClause) {
  char tmp[100];

  // template <typename T>
  // int foo() requires std::integral<T>;
  //
  // foo<int>();
  ASSERT_TRUE(Demangle("_Z3fooIiEivQsr3stdE8integralIT_E", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "foo<>()");
}

TEST(Demangle, FunctionWithTemplateParamRequiresClause) {
  char tmp[100];

  // template <typename T>
  //     requires std::integral<T>
  // int foo();
  //
  // foo<int>();
  ASSERT_TRUE(Demangle("_Z3fooIiQsr3stdE8integralIT_EEiv", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "foo<>()");
}

TEST(Demangle, FunctionWithTemplateParamAndFunctionRequiresClauses) {
  char tmp[100];

  // template <typename T>
  //     requires std::integral<T>
  // int foo() requires std::integral<T>;
  //
  // foo<int>();
  ASSERT_TRUE(Demangle("_Z3fooIiQsr3stdE8integralIT_EEivQsr3stdE8integralIS0_E",
                       tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "foo<>()");
}

TEST(Demangle, FunctionTemplateBacktracksOnMalformedRequiresClause) {
  char tmp[100];

  // template <typename T>
  // int foo(T);
  //
  // foo<int>(5);
  // Except there's an extra `Q` where the mangled requires clause would be.
  ASSERT_FALSE(Demangle("_Z3fooIiQEiT_", tmp, sizeof(tmp)));
}

TEST(Demangle, FunctionTemplateWithAutoParam) {
  char tmp[100];

  // template <auto>
  // void foo();
  //
  // foo<1>();
  ASSERT_TRUE(Demangle("_Z3fooITnDaLi1EEvv", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "foo<>()");
}

TEST(Demangle, FunctionTemplateWithNonTypeParamPack) {
  char tmp[100];

  // template <int&..., typename T>
  // void foo(T);
  //
  // foo(2);
  ASSERT_TRUE(Demangle("_Z3fooITpTnRiJEiEvT0_", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "foo<>()");
}

TEST(Demangle, FunctionTemplateTemplateParamWithConstrainedArg) {
  char tmp[100];

  // template <typename T>
  // concept True = true;
  //
  // template <typename T> requires True<T>
  // struct Fooer {};
  //
  // template <template <typename T> typename>
  // void foo() {}
  //
  // foo<Fooer>();
  ASSERT_TRUE(Demangle("_Z3fooITtTyE5FooerEvv", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "foo<>()");
}

TEST(Demangle, ConstrainedAutoInFunctionTemplate) {
  char tmp[100];

  // template <typename T> concept C = true;
  // template <C auto N> void f() {}
  // template void f<0>();
  ASSERT_TRUE(Demangle("_Z1fITnDk1CLi0EEvv", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "f<>()");
}

TEST(Demangle, NonTemplateBuiltinType) {
  char tmp[100];

  // void foo(__my_builtin_type t);
  //
  // foo({});
  ASSERT_TRUE(Demangle("_Z3foou17__my_builtin_type", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "foo()");
}

TEST(Demangle, SingleArgTemplateBuiltinType) {
  char tmp[100];

  // template <typename T>
  // __my_builtin_type<T> foo();
  //
  // foo<int>();
  ASSERT_TRUE(Demangle("_Z3fooIiEu17__my_builtin_typeIT_Ev", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "foo<>()");
}

TEST(Demangle, FailsOnTwoArgTemplateBuiltinType) {
  char tmp[100];

  // template <typename T, typename U>
  // __my_builtin_type<T, U> foo();
  //
  // foo<int, char>();
  ASSERT_FALSE(
      Demangle("_Z3fooIicEu17__my_builtin_typeIT_T0_Ev", tmp, sizeof(tmp)));
}

TEST(Demangle, TemplateTemplateParamSubstitution) {
  char tmp[100];

  // template <typename T>
  // concept True = true;
  //
  // template<std::integral T, T> struct Foolable {};
  // template<template<typename T, T> typename> void foo() {}
  //
  // template void foo<Foolable>();
  ASSERT_TRUE(Demangle("_Z3fooITtTyTnTL0__E8FoolableEvv", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "foo<>()");
}

TEST(Demangle, TemplateParamSubstitutionWithGenericLambda) {
  char tmp[100];

  // template <typename>
  // struct Fooer {
  //     template <typename>
  //     void foo(decltype([](auto x, auto y) {})) {}
  // };
  //
  // Fooer<int> f;
  // f.foo<int>({});
  ASSERT_TRUE(
      Demangle("_ZN5FooerIiE3fooIiEEvNS0_UlTL0__TL0_0_E_E", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "Fooer<>::foo<>()");
}

TEST(Demangle, LambdaRequiresTrue) {
  char tmp[100];

  // auto $_0::operator()<int>(int) const requires true
  ASSERT_TRUE(Demangle("_ZNK3$_0clIiEEDaT_QLb1E", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "$_0::operator()<>()");
}

TEST(Demangle, LambdaRequiresSimpleExpression) {
  char tmp[100];

  // auto $_0::operator()<int>(int) const requires 2 + 2 == 4
  ASSERT_TRUE(Demangle("_ZNK3$_0clIiEEDaT_QeqplLi2ELi2ELi4E",
                       tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "$_0::operator()<>()");
}

TEST(Demangle, LambdaRequiresRequiresExpressionContainingTrue) {
  char tmp[100];

  // auto $_0::operator()<int>(int) const requires requires { true; }
  ASSERT_TRUE(Demangle("_ZNK3$_0clIiEEDaT_QrqXLb1EE", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "$_0::operator()<>()");
}

TEST(Demangle, LambdaRequiresRequiresExpressionContainingConcept) {
  char tmp[100];

  // auto $_0::operator()<int>(int) const
  // requires requires { std::same_as<decltype(fp), int>; }
  ASSERT_TRUE(Demangle("_ZNK3$_0clIiEEDaT_QrqXsr3stdE7same_asIDtfp_EiEE",
                       tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "$_0::operator()<>()");
}

TEST(Demangle, LambdaRequiresRequiresExpressionContainingNoexceptExpression) {
  char tmp[100];

  // auto $_0::operator()<int>(int) const
  // requires requires { {fp + fp} noexcept; }
  ASSERT_TRUE(Demangle("_ZNK3$_0clIiEEDaT_QrqXplfp_fp_NE", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "$_0::operator()<>()");
}

TEST(Demangle, LambdaRequiresRequiresExpressionContainingReturnTypeConstraint) {
  char tmp[100];

  // auto $_0::operator()<int>(int) const
  // requires requires { {fp + fp} -> std::same_as<decltype(fp)>; }
  ASSERT_TRUE(Demangle("_ZNK3$_0clIiEEDaT_QrqXplfp_fp_RNSt7same_asIDtfp_EEEE",
                       tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "$_0::operator()<>()");
}

TEST(Demangle, LambdaRequiresRequiresExpressionWithBothNoexceptAndReturnType) {
  char tmp[100];

  // auto $_0::operator()<int>(int) const
  // requires requires { {fp + fp} noexcept -> std::same_as<decltype(fp)>; }
  ASSERT_TRUE(Demangle("_ZNK3$_0clIiEEDaT_QrqXplfp_fp_NRNSt7same_asIDtfp_EEEE",
                       tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "$_0::operator()<>()");
}

TEST(Demangle, LambdaRequiresRequiresExpressionContainingType) {
  char tmp[100];

  // auto $_0::operator()<S>(S) const
  // requires requires { typename S::T; }
  ASSERT_TRUE(Demangle("_ZNK3$_0clI1SEEDaT_QrqTNS2_1TEE", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "$_0::operator()<>()");
}

TEST(Demangle, LambdaRequiresRequiresExpressionNestingAnotherRequires) {
  char tmp[100];

  // auto $_0::operator()<int>(int) const requires requires { requires true; }
  ASSERT_TRUE(Demangle("_ZNK3$_0clIiEEDaT_QrqQLb1EE", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "$_0::operator()<>()");
}

TEST(Demangle, LambdaRequiresRequiresExpressionContainingTwoRequirements) {
  char tmp[100];

  // auto $_0::operator()<int>(int) const
  // requires requires { requires true; requires 2 + 2 == 4; }
  ASSERT_TRUE(Demangle("_ZNK3$_0clIiEEDaT_QrqXLb1EXeqplLi2ELi2ELi4EE",
                       tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "$_0::operator()<>()");
}

TEST(Demangle, RequiresExpressionWithItsOwnParameter) {
  char tmp[100];

  // S<requires (int) { fp + fp; }> f<int>(int)
  ASSERT_TRUE(Demangle("_Z1fIiE1SIXrQT__XplfL0p_fp_EEES1_", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "f<>()");
}

TEST(Demangle, LambdaWithExplicitTypeArgument) {
  char tmp[100];

  // Source:
  //
  // template <class T> T f(T t) {
  //   return []<class U>(U u) { return u + u; }(t);
  // }
  //
  // template int f<int>(int);
  //
  // Full LLVM demangling of the lambda call operator:
  //
  // auto int f<int>(int)::'lambda'<typename $T>(int)::
  // operator()<int>(int) const
  ASSERT_TRUE(Demangle("_ZZ1fIiET_S0_ENKUlTyS0_E_clIiEEDaS0_",
                       tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "f<>()::{lambda()#1}::operator()<>()");
}

TEST(Demangle, LambdaWithExplicitPackArgument) {
  char tmp[100];

  // Source:
  //
  // template <class T> T h(T t) {
  //   return []<class... U>(U... u) {
  //     return ((u + u) + ... + 0);
  //   }(t);
  // }
  //
  // template int h<int>(int);
  //
  // Full LLVM demangling of the lambda call operator:
  //
  // auto int f<int>(int)::'lambda'<typename ...$T>($T...)::
  // operator()<int>($T...) const
  ASSERT_TRUE(Demangle("_ZZ1fIiET_S0_ENKUlTpTyDpT_E_clIJiEEEDaS2_",
                       tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "f<>()::{lambda()#1}::operator()<>()");
}

TEST(Demangle, LambdaInClassMemberDefaultArgument) {
  char tmp[100];

  // Source:
  //
  // struct S {
  //   static auto f(void (*g)() = [] {}) { return g; }
  // };
  // void (*p)() = S::f();
  //
  // Full LLVM demangling of the lambda call operator:
  //
  // S::f(void (*)())::'lambda'()::operator()() const
  //
  // Full GNU binutils demangling:
  //
  // S::f(void (*)())::{default arg#1}::{lambda()#1}::operator()() const
  ASSERT_TRUE(Demangle("_ZZN1S1fEPFvvEEd_NKUlvE_clEv", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "S::f()::{default arg#1}::{lambda()#1}::operator()()");

  // The same but in the second rightmost default argument.
  ASSERT_TRUE(Demangle("_ZZN1S1fEPFvvEEd0_NKUlvE_clEv", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "S::f()::{default arg#2}::{lambda()#1}::operator()()");

  // Reject negative <(parameter) number> values.
  ASSERT_FALSE(Demangle("_ZZN1S1fEPFvvEEdn1_NKUlvE_clEv", tmp, sizeof(tmp)));
}

TEST(Demangle, SubstpackNotationForTroublesomeTemplatePack) {
  char tmp[100];

  // Source:
  //
  // template <template <class> class, template <class> class> struct B {};
  //
  // template <template <class> class... T> struct A {
  //   template <template <class> class... U> void f(B<T, U>&&...) {}
  // };
  //
  // template void A<>::f<>();
  //
  // LLVM can't demangle its own _SUBSTPACK_ notation.
  ASSERT_TRUE(Demangle("_ZN1AIJEE1fIJEEEvDpO1BI_SUBSTPACK_T_E",
                       tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "A<>::f<>()");
}

TEST(Demangle, TemplateTemplateParamAppearingAsBackrefFollowedByTemplateArgs) {
  char tmp[100];

  // Source:
  //
  // template <template <class> class C> struct W {
  //   template <class T> static decltype(C<T>::m()) f() { return {}; }
  // };
  //
  // template <class T> struct S { static int m() { return 0; } };
  // template decltype(S<int>::m()) W<S>::f<int>();
  ASSERT_TRUE(Demangle("_ZN1WI1SE1fIiEEDTclsrS0_IT_EE1mEEv", tmp, sizeof(tmp)));
  EXPECT_STREQ(tmp, "W<>::f<>()");
}

// Test corner cases of boundary conditions.
TEST(Demangle, CornerCases) {
  char tmp[10];
  EXPECT_TRUE(Demangle("_Z6foobarv", tmp, sizeof(tmp)));
  // sizeof("foobar()") == 9
  EXPECT_STREQ("foobar()", tmp);
  EXPECT_TRUE(Demangle("_Z6foobarv", tmp, 9));
  EXPECT_STREQ("foobar()", tmp);
  EXPECT_FALSE(Demangle("_Z6foobarv", tmp, 8));  // Not enough.
  EXPECT_FALSE(Demangle("_Z6foobarv", tmp, 1));
  EXPECT_FALSE(Demangle("_Z6foobarv", tmp, 0));
  EXPECT_FALSE(Demangle("_Z6foobarv", nullptr, 0));  // Should not cause SEGV.
  EXPECT_FALSE(Demangle("_Z1000000", tmp, 9));
}

// Test handling of functions suffixed with .clone.N, which is used
// by GCC 4.5.x (and our locally-modified version of GCC 4.4.x), and
// .constprop.N and .isra.N, which are used by GCC 4.6.x.  These
// suffixes are used to indicate functions which have been cloned
// during optimization.  We ignore these suffixes.
TEST(Demangle, Clones) {
  char tmp[20];
  EXPECT_TRUE(Demangle("_ZL3Foov", tmp, sizeof(tmp)));
  EXPECT_STREQ("Foo()", tmp);
  EXPECT_TRUE(Demangle("_ZL3Foov.clone.3", tmp, sizeof(tmp)));
  EXPECT_STREQ("Foo()", tmp);
  EXPECT_TRUE(Demangle("_ZL3Foov.constprop.80", tmp, sizeof(tmp)));
  EXPECT_STREQ("Foo()", tmp);
  EXPECT_TRUE(Demangle("_ZL3Foov.isra.18", tmp, sizeof(tmp)));
  EXPECT_STREQ("Foo()", tmp);
  EXPECT_TRUE(Demangle("_ZL3Foov.isra.2.constprop.18", tmp, sizeof(tmp)));
  EXPECT_STREQ("Foo()", tmp);
  // Demangle suffixes produced by -funique-internal-linkage-names.
  EXPECT_TRUE(Demangle("_ZL3Foov.__uniq.12345", tmp, sizeof(tmp)));
  EXPECT_STREQ("Foo()", tmp);
  EXPECT_TRUE(Demangle("_ZL3Foov.__uniq.12345.isra.2.constprop.18", tmp,
                       sizeof(tmp)));
  EXPECT_STREQ("Foo()", tmp);
  // Suffixes without the number should also demangle.
  EXPECT_TRUE(Demangle("_ZL3Foov.clo", tmp, sizeof(tmp)));
  EXPECT_STREQ("Foo()", tmp);
  // Suffixes with just the number should also demangle.
  EXPECT_TRUE(Demangle("_ZL3Foov.123", tmp, sizeof(tmp)));
  EXPECT_STREQ("Foo()", tmp);
  // (.clone. followed by non-number), should also demangle.
  EXPECT_TRUE(Demangle("_ZL3Foov.clone.foo", tmp, sizeof(tmp)));
  EXPECT_STREQ("Foo()", tmp);
  // (.clone. followed by multiple numbers), should also demangle.
  EXPECT_TRUE(Demangle("_ZL3Foov.clone.123.456", tmp, sizeof(tmp)));
  EXPECT_STREQ("Foo()", tmp);
  // (a long valid suffix), should demangle.
  EXPECT_TRUE(Demangle("_ZL3Foov.part.9.165493.constprop.775.31805", tmp,
                       sizeof(tmp)));
  EXPECT_STREQ("Foo()", tmp);
  // Invalid (. without anything else), should not demangle.
  EXPECT_FALSE(Demangle("_ZL3Foov.", tmp, sizeof(tmp)));
  // Invalid (. with mix of alpha and digits), should not demangle.
  EXPECT_FALSE(Demangle("_ZL3Foov.abc123", tmp, sizeof(tmp)));
  // Invalid (.clone. not followed by number), should not demangle.
  EXPECT_FALSE(Demangle("_ZL3Foov.clone.", tmp, sizeof(tmp)));
  // Invalid (.constprop. not followed by number), should not demangle.
  EXPECT_FALSE(Demangle("_ZL3Foov.isra.2.constprop.", tmp, sizeof(tmp)));
}

TEST(Demangle, Discriminators) {
  char tmp[80];

  // Source:
  //
  // using Thunk = void (*)();
  //
  // Thunk* f() {
  //   static Thunk thunks[12] = {};
  //
  // #define THUNK(i) [backslash here]
  //   do { struct S { static void g() {} }; thunks[i] = &S::g; } while (0)
  //
  //   THUNK(0);
  //   [... repeat for 1 to 10 ...]
  //   THUNK(11);
  //
  //   return thunks;
  // }
  //
  // The test inputs are manglings of some of the S::g member functions.

  // The first one omits the discriminator.
  EXPECT_TRUE(Demangle("_ZZ1fvEN1S1gEv", tmp, sizeof(tmp)));
  EXPECT_STREQ("f()::S::g()", tmp);

  // The second one encodes 0.
  EXPECT_TRUE(Demangle("_ZZ1fvEN1S1gE_0v", tmp, sizeof(tmp)));
  EXPECT_STREQ("f()::S::g()", tmp);

  // The eleventh one encodes 9.
  EXPECT_TRUE(Demangle("_ZZ1fvEN1S1gE_9v", tmp, sizeof(tmp)));
  EXPECT_STREQ("f()::S::g()", tmp);

  // The twelfth one encodes 10 with extra underscores delimiting it.
  EXPECT_TRUE(Demangle("_ZZ1fvEN1S1gE__10_v", tmp, sizeof(tmp)));
  EXPECT_STREQ("f()::S::g()", tmp);
}

TEST(Demangle, SingleDigitDiscriminatorFollowedByADigit) {
  char tmp[80];

  // Don't parse 911 as a number.
  EXPECT_TRUE(Demangle("_ZZ1fvEN1S1gE_911return_type", tmp, sizeof(tmp)));
  EXPECT_STREQ("f()::S::g()", tmp);
}

TEST(Demangle, LiteralOfGlobalNamespaceEnumType) {
  char tmp[80];

  // void f<(E)42>()
  EXPECT_TRUE(Demangle("_Z1fIL1E42EEvv", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);
}

TEST(Demangle, NullptrLiterals) {
  char tmp[80];

  // void f<nullptr>()
  EXPECT_TRUE(Demangle("_Z1fILDnEEvv", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);

  // also void f<nullptr>()
  EXPECT_TRUE(Demangle("_Z1fILDn0EEvv", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);
}

TEST(Demangle, StringLiterals) {
  char tmp[80];

  // void f<"<char const [42]>">()
  EXPECT_TRUE(Demangle("_Z1fILA42_KcEEvv", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);
}

// Test the GNU abi_tag extension.
TEST(Demangle, AbiTags) {
  char tmp[80];

  // Mangled name generated via:
  // struct [[gnu::abi_tag("abc")]] A{};
  // A a;
  EXPECT_TRUE(Demangle("_Z1aB3abc", tmp, sizeof(tmp)));
  EXPECT_STREQ("a[abi:abc]", tmp);

  // Mangled name generated via:
  // struct B {
  //   B [[gnu::abi_tag("xyz")]] (){};
  // };
  // B b;
  EXPECT_TRUE(Demangle("_ZN1BC2B3xyzEv", tmp, sizeof(tmp)));
  EXPECT_STREQ("B::B[abi:xyz]()", tmp);

  // Mangled name generated via:
  // [[gnu::abi_tag("foo", "bar")]] void C() {}
  EXPECT_TRUE(Demangle("_Z1CB3barB3foov", tmp, sizeof(tmp)));
  EXPECT_STREQ("C[abi:bar][abi:foo]()", tmp);
}

TEST(Demangle, EnableIfAttributeOnGlobalFunction) {
  char tmp[80];

  // int f(long l) __attribute__((enable_if(l >= 0, ""))) { return l; }
  //
  // f(long) [enable_if:fp >= 0]
  EXPECT_TRUE(Demangle("_Z1fUa9enable_ifIXgefL0p_Li0EEEl", tmp, sizeof(tmp)));
  EXPECT_STREQ("f()", tmp);
}

TEST(Demangle, EnableIfAttributeOnNamespaceScopeFunction) {
  char tmp[80];

  // namespace ns {
  // int f(long l) __attribute__((enable_if(l >= 0, ""))) { return l; }
  // }  // namespace ns
  //
  // ns::f(long) [enable_if:fp >= 0]
  EXPECT_TRUE(Demangle("_ZN2ns1fEUa9enable_ifIXgefL0p_Li0EEEl",
              tmp, sizeof(tmp)));
  EXPECT_STREQ("ns::f()", tmp);
}

TEST(Demangle, EnableIfAttributeOnFunctionTemplate) {
  char tmp[80];

  // template <class T>
  // T f(T t) __attribute__((enable_if(t >= T{}, ""))) { return t; }
  // template int f<int>(int);
  //
  // int f<int>(int) [enable_if:fp >= int{}]
  EXPECT_TRUE(Demangle("_Z1fIiEUa9enable_ifIXgefL0p_tliEEET_S0_",
              tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);
}

TEST(Demangle, ThisPointerInDependentSignature) {
  char tmp[80];

  // decltype(g<int>(this)) S::f<int>()
  EXPECT_TRUE(Demangle("_ZN1S1fIiEEDTcl1gIT_EfpTEEv", tmp, sizeof(tmp)));
  EXPECT_STREQ("S::f<>()", tmp);
}

TEST(Demangle, DependentMemberOperatorCall) {
  char tmp[80];

  // decltype(fp.operator()()) f<C>(C)
  EXPECT_TRUE(Demangle("_Z1fI1CEDTcldtfp_onclEET_", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);
}

TEST(Demangle, TypeNestedUnderDecltype) {
  char tmp[80];

  // Source:
  //
  // template <class T> struct S { using t = int; };
  // template <class T> decltype(S<T>{})::t f() { return {}; }
  // void g() { f<int>(); }
  //
  // Full LLVM demangling of the instantiation of f:
  //
  // decltype(S<int>{})::t f<int>()
  EXPECT_TRUE(Demangle("_Z1fIiENDTtl1SIT_EEE1tEv", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);
}

// Test subobject-address template parameters.
TEST(Demangle, SubobjectAddresses) {
  char tmp[80];

  // void f<a.<char const at offset 123>>()
  EXPECT_TRUE(Demangle("_Z1fIXsoKcL_Z1aE123EEEvv", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);

  // void f<&a.<char const at offset 0>>()
  EXPECT_TRUE(Demangle("_Z1fIXadsoKcL_Z1aEEEEvv", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);

  // void f<&a.<char const at offset 123>>()
  EXPECT_TRUE(Demangle("_Z1fIXadsoKcL_Z1aE123EEEvv", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);

  // void f<&a.<char const at offset 123>>(), past the end this time
  EXPECT_TRUE(Demangle("_Z1fIXadsoKcL_Z1aE123pEEEvv", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);

  // void f<&a.<char const at offset 0>>() with union-selectors
  EXPECT_TRUE(Demangle("_Z1fIXadsoKcL_Z1aE__1_234EEEvv", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);

  // void f<&a.<char const at offset 123>>(), past the end, with union-selector
  EXPECT_TRUE(Demangle("_Z1fIXadsoKcL_Z1aE123_456pEEEvv", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);
}

TEST(Demangle, UnaryFoldExpressions) {
  char tmp[80];

  // Source:
  //
  // template <bool b> struct S {};
  //
  // template <class... T> auto f(T... t) -> S<((sizeof(T) == 4) || ...)> {
  //   return {};
  // }
  //
  // void g() { f(1, 2L); }
  //
  // Full LLVM demangling of the instantiation of f:
  //
  // S<((sizeof (int) == 4, sizeof (long) == 4) || ...)> f<int, long>(int, long)
  EXPECT_TRUE(Demangle("_Z1fIJilEE1SIXfrooeqstT_Li4EEEDpS1_",
                       tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);

  // The like with a left fold.
  //
  // S<(... || (sizeof (int) == 4, sizeof (long) == 4))> f<int, long>(int, long)
  EXPECT_TRUE(Demangle("_Z1fIJilEE1SIXflooeqstT_Li4EEEDpS1_",
                       tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);
}

TEST(Demangle, BinaryFoldExpressions) {
  char tmp[80];

  // Source:
  //
  // template <bool b> struct S {};
  //
  // template <class... T> auto f(T... t)
  //     -> S<((sizeof(T) == 4) || ... || false)> {
  //   return {};
  // }
  //
  // void g() { f(1, 2L); }
  //
  // Full LLVM demangling of the instantiation of f:
  //
  // S<((sizeof (int) == 4, sizeof (long) == 4) || ... || false)>
  // f<int, long>(int, long)
  EXPECT_TRUE(Demangle("_Z1fIJilEE1SIXfRooeqstT_Li4ELb0EEEDpS1_",
                       tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);

  // The like with a left fold.
  //
  // S<(false || ... || (sizeof (int) == 4, sizeof (long) == 4))>
  // f<int, long>(int, long)
  EXPECT_TRUE(Demangle("_Z1fIJilEE1SIXfLooLb0EeqstT_Li4EEEDpS1_",
                       tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);
}

TEST(Demangle, SizeofPacks) {
  char tmp[80];

  // template <std::size_t i> struct S {};
  //
  // template <class... T> auto f(T... p) -> S<sizeof...(T)> { return {}; }
  // template auto f<int, long>(int, long) -> S<2>;
  //
  // template <class... T> auto g(T... p) -> S<sizeof...(p)> { return {}; }
  // template auto g<int, long>(int, long) -> S<2>;

  // S<sizeof...(int, long)> f<int, long>(int, long)
  EXPECT_TRUE(Demangle("_Z1fIJilEE1SIXsZT_EEDpT_", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);

  // S<sizeof... (fp)> g<int, long>(int, long)
  EXPECT_TRUE(Demangle("_Z1gIJilEE1SIXsZfp_EEDpT_", tmp, sizeof(tmp)));
  EXPECT_STREQ("g<>()", tmp);
}

TEST(Demangle, Spaceship) {
  char tmp[80];

  // #include <compare>
  //
  // struct S { auto operator<=>(const S&) const = default; };
  // auto (S::*f) = &S::operator<=>;  // make sure S::operator<=> is emitted
  //
  // template <class T> auto g(T x, T y) -> decltype(x <=> y) {
  //   return x <=> y;
  // }
  // template auto g<S>(S x, S y) -> decltype(x <=> y);

  // S::operator<=>(S const&) const
  EXPECT_TRUE(Demangle("_ZNK1SssERKS_", tmp, sizeof(tmp)));
  EXPECT_STREQ("S::operator<=>()", tmp);

  // decltype(fp <=> fp0) g<S>(S, S)
  EXPECT_TRUE(Demangle("_Z1gI1SEDTssfp_fp0_ET_S2_", tmp, sizeof(tmp)));
  EXPECT_STREQ("g<>()", tmp);
}

TEST(Demangle, VendorExtendedExpressions) {
  char tmp[80];

  // void f<__e()>()
  EXPECT_TRUE(Demangle("_Z1fIXu3__eEEEvv", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);

  // void f<__e(int, long)>()
  EXPECT_TRUE(Demangle("_Z1fIXu3__eilEEEvv", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);
}

TEST(Demangle, DirectListInitialization) {
  char tmp[80];

  // template <class T> decltype(T{}) f() { return T{}; }
  // template decltype(int{}) f<int>();
  //
  // struct XYZ { int x, y, z; };
  // template <class T> decltype(T{1, 2, 3}) g() { return T{1, 2, 3}; }
  // template decltype(XYZ{1, 2, 3}) g<XYZ>();
  //
  // template <class T> decltype(T{.x = 1, .y = 2, .z = 3}) h() {
  //   return T{.x = 1, .y = 2, .z = 3};
  // }
  // template decltype(XYZ{.x = 1, .y = 2, .z = 3}) h<XYZ>();
  //
  // // The following two cases require full C99 designated initializers,
  // // not part of C++ but likely available as an extension if you ask your
  // // compiler nicely.
  //
  // struct A { int a[4]; };
  // template <class T> decltype(T{.a[2] = 42}) i() { return T{.a[2] = 42}; }
  // template decltype(A{.a[2] = 42}) i<A>();
  //
  // template <class T> decltype(T{.a[1 ... 3] = 42}) j() {
  //   return T{.a[1 ... 3] = 42};
  // }
  // template decltype(A{.a[1 ... 3] = 42}) j<A>();

  // decltype(int{}) f<int>()
  EXPECT_TRUE(Demangle("_Z1fIiEDTtlT_EEv", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);

  // decltype(XYZ{1, 2, 3}) g<XYZ>()
  EXPECT_TRUE(Demangle("_Z1gI3XYZEDTtlT_Li1ELi2ELi3EEEv", tmp, sizeof(tmp)));
  EXPECT_STREQ("g<>()", tmp);

  // decltype(XYZ{.x = 1, .y = 2, .z = 3}) h<XYZ>()
  EXPECT_TRUE(Demangle("_Z1hI3XYZEDTtlT_di1xLi1Edi1yLi2Edi1zLi3EEEv",
                       tmp, sizeof(tmp)));
  EXPECT_STREQ("h<>()", tmp);

  // decltype(A{.a[2] = 42}) i<A>()
  EXPECT_TRUE(Demangle("_Z1iI1AEDTtlT_di1adxLi2ELi42EEEv", tmp, sizeof(tmp)));
  EXPECT_STREQ("i<>()", tmp);

  // decltype(A{.a[1 ... 3] = 42}) j<A>()
  EXPECT_TRUE(Demangle("_Z1jI1AEDTtlT_di1adXLi1ELi3ELi42EEEv",
                       tmp, sizeof(tmp)));
  EXPECT_STREQ("j<>()", tmp);
}

TEST(Demangle, ReferenceQualifiedFunctionTypes) {
  char tmp[80];

  // void f(void (*)() const &, int)
  EXPECT_TRUE(Demangle("_Z1fPKFvvREi", tmp, sizeof(tmp)));
  EXPECT_STREQ("f()", tmp);

  // void f(void (*)() &&, int)
  EXPECT_TRUE(Demangle("_Z1fPFvvOEi", tmp, sizeof(tmp)));
  EXPECT_STREQ("f()", tmp);

  // void f(void (*)(int&) &, int)
  EXPECT_TRUE(Demangle("_Z1fPFvRiREi", tmp, sizeof(tmp)));
  EXPECT_STREQ("f()", tmp);

  // void f(void (*)(S&&) &&, int)
  EXPECT_TRUE(Demangle("_Z1fPFvO1SOEi", tmp, sizeof(tmp)));
  EXPECT_STREQ("f()", tmp);
}

TEST(Demangle, DynamicCast) {
  char tmp[80];

  // Source:
  //
  // template <class T> auto f(T* p) -> decltype(dynamic_cast<const T*>(p)) {
  //   return p;
  // }
  // struct S {};
  // void g(S* p) { f(p); }
  //
  // Full LLVM demangling of the instantiation of f:
  //
  // decltype(dynamic_cast<S const*>(fp)) f<S>(S*)
  EXPECT_TRUE(Demangle("_Z1fI1SEDTdcPKT_fp_EPS1_", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);
}

TEST(Demangle, StaticCast) {
  char tmp[80];

  // Source:
  //
  // template <class T> auto f(T* p) -> decltype(static_cast<const T*>(p)) {
  //   return p;
  // }
  // void g(int* p) { f(p); }
  //
  // Full LLVM demangling of the instantiation of f:
  //
  // decltype(static_cast<int const*>(fp)) f<int>(int*)
  EXPECT_TRUE(Demangle("_Z1fIiEDTscPKT_fp_EPS0_", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);
}

TEST(Demangle, ConstCast) {
  char tmp[80];

  // Source:
  //
  // template <class T> auto f(T* p) -> decltype(const_cast<const T*>(p)) {
  //   return p;
  // }
  // void g(int* p) { f(p); }
  //
  // Full LLVM demangling of the instantiation of f:
  //
  // decltype(const_cast<int const*>(fp)) f<int>(int*)
  EXPECT_TRUE(Demangle("_Z1fIiEDTccPKT_fp_EPS0_", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);
}

TEST(Demangle, ReinterpretCast) {
  char tmp[80];

  // Source:
  //
  // template <class T> auto f(T* p)
  //     -> decltype(reinterpret_cast<const T*>(p)) {
  //   return p;
  // }
  // void g(int* p) { f(p); }
  //
  // Full LLVM demangling of the instantiation of f:
  //
  // decltype(reinterpret_cast<int const*>(fp)) f<int>(int*)
  EXPECT_TRUE(Demangle("_Z1fIiEDTrcPKT_fp_EPS0_", tmp, sizeof(tmp)));
  EXPECT_STREQ("f<>()", tmp);
}

TEST(Demangle, ThreadLocalWrappers) {
  char tmp[80];

  EXPECT_TRUE(Demangle("_ZTWN2ns3varE", tmp, sizeof(tmp)));
  EXPECT_STREQ("thread-local wrapper routine for ns::var", tmp);

  EXPECT_TRUE(Demangle("_ZTHN2ns3varE", tmp, sizeof(tmp)));
  EXPECT_STREQ("thread-local initialization routine for ns::var", tmp);
}

// Test one Rust symbol to exercise Demangle's delegation path.  Rust demangling
// itself is more thoroughly tested in demangle_rust_test.cc.
TEST(Demangle, DelegatesToDemangleRustSymbolEncoding) {
  char tmp[80];

  EXPECT_TRUE(Demangle("_RNvC8my_crate7my_func", tmp, sizeof(tmp)));
  EXPECT_STREQ("my_crate::my_func", tmp);
}

// Tests that verify that Demangle footprint is within some limit.
// They are not to be run under sanitizers as the sanitizers increase
// stack consumption by about 4x.
#if defined(ABSL_INTERNAL_HAVE_DEBUGGING_STACK_CONSUMPTION) && \
    !defined(ABSL_HAVE_ADDRESS_SANITIZER) &&                   \
    !defined(ABSL_HAVE_MEMORY_SANITIZER) &&                    \
    !defined(ABSL_HAVE_THREAD_SANITIZER)

static const char *g_mangled;
static char g_demangle_buffer[4096];
static char *g_demangle_result;

static void DemangleSignalHandler(int signo) {
  if (Demangle(g_mangled, g_demangle_buffer, sizeof(g_demangle_buffer))) {
    g_demangle_result = g_demangle_buffer;
  } else {
    g_demangle_result = nullptr;
  }
}

// Call Demangle and figure out the stack footprint of this call.
static const char *DemangleStackConsumption(const char *mangled,
                                            int *stack_consumed) {
  g_mangled = mangled;
  *stack_consumed = GetSignalHandlerStackConsumption(DemangleSignalHandler);
  LOG(INFO) << "Stack consumption of Demangle: " << *stack_consumed;
  return g_demangle_result;
}

// Demangle stack consumption should be within 8kB for simple mangled names
// with some level of nesting. With alternate signal stack we have 64K,
// but some signal handlers run on thread stack, and could have arbitrarily
// little space left (so we don't want to make this number too large).
const int kStackConsumptionUpperLimit = 8192;

// Returns a mangled name nested to the given depth.
static std::string NestedMangledName(int depth) {
  std::string mangled_name = "_Z1a";
  if (depth > 0) {
    mangled_name += "IXL";
    mangled_name += NestedMangledName(depth - 1);
    mangled_name += "EEE";
  }
  return mangled_name;
}

TEST(Demangle, DemangleStackConsumption) {
  // Measure stack consumption of Demangle for nested mangled names of varying
  // depth.  Since Demangle is implemented as a recursive descent parser,
  // stack consumption will grow as the nesting depth increases.  By measuring
  // the stack consumption for increasing depths, we can see the growing
  // impact of any stack-saving changes made to the code for Demangle.
  int stack_consumed = 0;

  const char *demangled =
      DemangleStackConsumption("_Z6foobarv", &stack_consumed);
  EXPECT_STREQ("foobar()", demangled);
  EXPECT_GT(stack_consumed, 0);
  EXPECT_LT(stack_consumed, kStackConsumptionUpperLimit);

  const std::string nested_mangled_name0 = NestedMangledName(0);
  demangled = DemangleStackConsumption(nested_mangled_name0.c_str(),
                                       &stack_consumed);
  EXPECT_STREQ("a", demangled);
  EXPECT_GT(stack_consumed, 0);
  EXPECT_LT(stack_consumed, kStackConsumptionUpperLimit);

  const std::string nested_mangled_name1 = NestedMangledName(1);
  demangled = DemangleStackConsumption(nested_mangled_name1.c_str(),
                                       &stack_consumed);
  EXPECT_STREQ("a<>", demangled);
  EXPECT_GT(stack_consumed, 0);
  EXPECT_LT(stack_consumed, kStackConsumptionUpperLimit);

  const std::string nested_mangled_name2 = NestedMangledName(2);
  demangled = DemangleStackConsumption(nested_mangled_name2.c_str(),
                                       &stack_consumed);
  EXPECT_STREQ("a<>", demangled);
  EXPECT_GT(stack_consumed, 0);
  EXPECT_LT(stack_consumed, kStackConsumptionUpperLimit);

  const std::string nested_mangled_name3 = NestedMangledName(3);
  demangled = DemangleStackConsumption(nested_mangled_name3.c_str(),
                                       &stack_consumed);
  EXPECT_STREQ("a<>", demangled);
  EXPECT_GT(stack_consumed, 0);
  EXPECT_LT(stack_consumed, kStackConsumptionUpperLimit);
}

#endif  // Stack consumption tests

static void TestOnInput(const char* input) {
  static const int kOutSize = 1048576;
  auto out = absl::make_unique<char[]>(kOutSize);
  Demangle(input, out.get(), kOutSize);
}

TEST(DemangleRegression, NegativeLength) {
  TestOnInput("_ZZn4");
}

TEST(DemangleRegression, DeeplyNestedArrayType) {
  const int depth = 100000;
  std::string data = "_ZStI";
  data.reserve(data.size() + 3 * depth + 1);
  for (int i = 0; i < depth; i++) {
    data += "A1_";
  }
  TestOnInput(data.c_str());
}

struct Base {
  virtual ~Base() = default;
};

struct Derived : public Base {};

TEST(DemangleStringTest, SupportsSymbolNameReturnedByTypeId) {
  EXPECT_EQ(DemangleString(typeid(int).name()), "int");
  // We want to test that `DemangleString` can demangle the symbol names
  // returned by `typeid`, but without hard-coding the actual demangled values
  // (because they are platform-specific).
  EXPECT_THAT(
      DemangleString(typeid(Base).name()),
      ContainsRegex("absl.*debugging_internal.*anonymous namespace.*::Base"));
  EXPECT_THAT(DemangleString(typeid(Derived).name()),
              ContainsRegex(
                  "absl.*debugging_internal.*anonymous namespace.*::Derived"));
}

}  // namespace
}  // namespace debugging_internal
ABSL_NAMESPACE_END
}  // namespace absl
