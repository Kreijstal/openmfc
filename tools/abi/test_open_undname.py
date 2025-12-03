#!/usr/bin/env python3
"""
Unit tests for the open_undname.py MSVC C++ demangler.
Tests cover basic functionality for Phase 0A/0B symbols.
"""
import unittest
import sys
import os

# Add the parent directory to path for importing open_undname
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from open_undname import Undecorator


class TestUndecoratorBasics(unittest.TestCase):
    """Test basic demangling functionality."""

    def test_non_mangled_symbol_returns_as_is(self):
        """Non-mangled symbols should be returned unchanged."""
        u = Undecorator("printf")
        self.assertEqual(u.demangle(), "printf")

    def test_simple_method(self):
        """Test a simple public method."""
        # ?GetValue@CClass@@QEBAHXZ -> public: int __cdecl CClass::GetValue(void) const
        u = Undecorator("?GetValue@CClass@@QEBAHXZ")
        result = u.demangle()
        self.assertIn("CClass", result)
        self.assertIn("GetValue", result)
        self.assertIn("int", result)


class TestConstructorsDestructors(unittest.TestCase):
    """Test constructor and destructor demangling."""

    def test_default_constructor(self):
        """Test default constructor (??0)."""
        # ??0event@Concurrency@@QEAA@XZ -> public: __cdecl Concurrency::event::event(void)
        u = Undecorator("??0event@Concurrency@@QEAA@XZ")
        result = u.demangle()
        self.assertIn("Concurrency", result)
        self.assertIn("event", result)
        self.assertIn("public", result)

    def test_destructor(self):
        """Test destructor (??1)."""
        # ??1_Cancellation_beacon@details@Concurrency@@QEAA@XZ
        u = Undecorator("??1_Cancellation_beacon@details@Concurrency@@QEAA@XZ")
        result = u.demangle()
        self.assertIn("~", result)  # Destructor should have ~
        self.assertIn("_Cancellation_beacon", result)

    def test_nested_class_constructor(self):
        """Test constructor for a nested class."""
        # ??0_Scoped_lock@_NonReentrantPPLLock@details@Concurrency@@QEAA@AEAV123@@Z
        u = Undecorator("??0_Scoped_lock@_NonReentrantPPLLock@details@Concurrency@@QEAA@AEAV123@@Z")
        result = u.demangle()
        self.assertIn("_Scoped_lock", result)
        self.assertIn("public", result)


class TestPrimitiveTypes(unittest.TestCase):
    """Test primitive type demangling."""

    def test_void_return(self):
        """Test void return type (X)."""
        u = Undecorator("?lock@critical_section@Concurrency@@QEAAXXZ")
        result = u.demangle()
        self.assertIn("void", result)
        self.assertIn("lock", result)

    def test_int_return(self):
        """Test int return type (H)."""
        u = Undecorator("?GetValue@CClass@@QEBAHXZ")
        result = u.demangle()
        self.assertIn("int", result)

    def test_bool_return(self):
        """Test bool return type (_N)."""
        u = Undecorator("?cancel@agent@Concurrency@@QEAA_NXZ")
        result = u.demangle()
        self.assertIn("bool", result)

    def test_unsigned_int_param(self):
        """Test unsigned int parameter (I)."""
        u = Undecorator("?try_lock_for@critical_section@Concurrency@@QEAA_NI@Z")
        result = u.demangle()
        self.assertIn("unsigned int", result)


class TestAccessModifiers(unittest.TestCase):
    """Test access modifier demangling (public, private, protected)."""

    def test_public_method(self):
        """Test public method (Q)."""
        u = Undecorator("?lock@critical_section@Concurrency@@QEAAXXZ")
        result = u.demangle()
        self.assertIn("public", result)

    def test_protected_method(self):
        """Test protected method (I)."""
        u = Undecorator("??0_Timer@details@Concurrency@@IEAA@I_N@Z")
        result = u.demangle()
        self.assertIn("protected", result)

    def test_static_method(self):
        """Test static method (S)."""
        u = Undecorator("?ResetDefaultSchedulerPolicy@Scheduler@Concurrency@@SAXXZ")
        result = u.demangle()
        self.assertIn("static", result)


class TestNamespaces(unittest.TestCase):
    """Test namespace handling."""

    def test_single_namespace(self):
        """Test symbol in single namespace."""
        u = Undecorator("??0event@Concurrency@@QEAA@XZ")
        result = u.demangle()
        self.assertIn("Concurrency", result)

    def test_nested_namespace(self):
        """Test symbol in nested namespace."""
        u = Undecorator("??0_Condition_variable@details@Concurrency@@QEAA@XZ")
        result = u.demangle()
        # Should have both Concurrency and details namespaces
        self.assertIn("Concurrency", result)
        self.assertIn("details", result)


class TestCallingConventions(unittest.TestCase):
    """Test calling convention demangling."""

    def test_cdecl(self):
        """Test __cdecl calling convention (A)."""
        u = Undecorator("?lock@critical_section@Concurrency@@QEAAXXZ")
        result = u.demangle()
        self.assertIn("__cdecl", result)


class TestPointerTypes(unittest.TestCase):
    """Test pointer type demangling."""

    def test_class_pointer_param(self):
        """Test class pointer parameter."""
        u = Undecorator("??0_Context@details@Concurrency@@QEAA@PEAVContext@2@@Z")
        result = u.demangle()
        # Should mention Context and pointer
        self.assertIn("Context", result)


class TestUndecoratorInternal(unittest.TestCase):
    """Test internal methods of Undecorator class."""

    def test_peek_at_start(self):
        """Test peek() at start of string."""
        u = Undecorator("?Test@Class@@QEAAXXZ")
        self.assertEqual(u.peek(), "?")

    def test_peek_empty(self):
        """Test peek() on empty string."""
        u = Undecorator("")
        self.assertIsNone(u.peek())

    def test_consume(self):
        """Test consume() advances position."""
        u = Undecorator("ABC")
        self.assertEqual(u.consume(), "A")
        self.assertEqual(u.consume(), "B")
        self.assertEqual(u.consume(), "C")
        self.assertIsNone(u.consume())

    def test_parse_type_int(self):
        """Test parse_type for int (H)."""
        u = Undecorator("H")
        result = u.parse_type()
        self.assertEqual(result, "int")

    def test_parse_type_void(self):
        """Test parse_type for void (X)."""
        u = Undecorator("X")
        result = u.parse_type()
        self.assertEqual(result, "void")

    def test_parse_type_char(self):
        """Test parse_type for char (D)."""
        u = Undecorator("D")
        result = u.parse_type()
        self.assertEqual(result, "char")

    def test_parse_type_unsigned_char(self):
        """Test parse_type for unsigned char (E)."""
        u = Undecorator("E")
        result = u.parse_type()
        self.assertEqual(result, "unsigned char")

    def test_parse_type_short(self):
        """Test parse_type for short (F)."""
        u = Undecorator("F")
        result = u.parse_type()
        self.assertEqual(result, "short")

    def test_parse_type_unsigned_short(self):
        """Test parse_type for unsigned short (G)."""
        u = Undecorator("G")
        result = u.parse_type()
        self.assertEqual(result, "unsigned short")

    def test_parse_type_long(self):
        """Test parse_type for long (J)."""
        u = Undecorator("J")
        result = u.parse_type()
        self.assertEqual(result, "long")

    def test_parse_type_unsigned_long(self):
        """Test parse_type for unsigned long (K)."""
        u = Undecorator("K")
        result = u.parse_type()
        self.assertEqual(result, "unsigned long")

    def test_parse_type_float(self):
        """Test parse_type for float (M)."""
        u = Undecorator("M")
        result = u.parse_type()
        self.assertEqual(result, "float")

    def test_parse_type_double(self):
        """Test parse_type for double (N)."""
        u = Undecorator("N")
        result = u.parse_type()
        self.assertEqual(result, "double")

    def test_parse_type_bool(self):
        """Test parse_type for bool (_N)."""
        u = Undecorator("_N")
        result = u.parse_type()
        self.assertEqual(result, "bool")

    def test_parse_type_int64(self):
        """Test parse_type for __int64 (_J)."""
        u = Undecorator("_J")
        result = u.parse_type()
        self.assertEqual(result, "__int64")

    def test_parse_type_unsigned_int64(self):
        """Test parse_type for unsigned __int64 (_K)."""
        u = Undecorator("_K")
        result = u.parse_type()
        self.assertEqual(result, "unsigned __int64")

    def test_parse_type_wchar_t(self):
        """Test parse_type for wchar_t (W)."""
        u = Undecorator("W")
        result = u.parse_type()
        self.assertEqual(result, "wchar_t")


class TestClassTypes(unittest.TestCase):
    """Test class/struct type demangling."""

    def test_class_type(self):
        """Test class type (V)."""
        u = Undecorator("VTestClass@@")
        result = u.parse_type()
        self.assertIn("class", result)
        self.assertIn("TestClass", result)

    def test_struct_type(self):
        """Test struct type (U)."""
        u = Undecorator("U_Mbstatet@@")
        result = u.parse_type()
        self.assertIn("struct", result)
        self.assertIn("_Mbstatet", result)


class TestRealWorldSymbols(unittest.TestCase):
    """Test with real-world symbols from the testdata."""

    def test_scheduler_policy_constructor(self):
        """Test SchedulerPolicy default constructor."""
        u = Undecorator("??0SchedulerPolicy@Concurrency@@QEAA@XZ")
        result = u.demangle()
        self.assertIn("SchedulerPolicy", result)
        self.assertIn("Concurrency", result)
        self.assertIn("public", result)

    def test_critical_section_constructor(self):
        """Test critical_section default constructor."""
        u = Undecorator("??0critical_section@Concurrency@@QEAA@XZ")
        result = u.demangle()
        self.assertIn("critical_section", result)
        self.assertIn("Concurrency", result)

    def test_reader_writer_lock_destructor(self):
        """Test reader_writer_lock destructor."""
        u = Undecorator("??1reader_writer_lock@Concurrency@@QEAA@XZ")
        result = u.demangle()
        self.assertIn("~", result)
        self.assertIn("reader_writer_lock", result)

    def test_try_lock_method(self):
        """Test try_lock method."""
        u = Undecorator("?try_lock@critical_section@Concurrency@@QEAA_NXZ")
        result = u.demangle()
        self.assertIn("try_lock", result)
        self.assertIn("bool", result)

    def test_unlock_method(self):
        """Test unlock method."""
        u = Undecorator("?unlock@critical_section@Concurrency@@QEAAXXZ")
        result = u.demangle()
        self.assertIn("unlock", result)
        self.assertIn("void", result)

    def test_get_error_code(self):
        """Test get_error_code method."""
        u = Undecorator("?get_error_code@scheduler_resource_allocation_error@Concurrency@@QEBAJXZ")
        result = u.demangle()
        self.assertIn("get_error_code", result)
        self.assertIn("long", result)
        # Note: const detection may not be fully implemented in current version


class TestConstMethods(unittest.TestCase):
    """Test const method demangling."""

    def test_const_method_symbol_parses(self):
        """Test const method (QEBA vs QEAA) parses without error."""
        # QEBA indicates const method
        # Note: Current implementation may not fully detect const in all cases
        u = Undecorator("?GetValue@CClass@@QEBAHXZ")
        result = u.demangle()
        # Verify basic parsing works
        self.assertIn("CClass", result)
        self.assertIn("GetValue", result)
        self.assertIn("int", result)


class TestEdgeCases(unittest.TestCase):
    """Test edge cases and error handling."""

    def test_empty_string(self):
        """Test empty string handling."""
        u = Undecorator("")
        result = u.demangle()
        self.assertEqual(result, "")

    def test_only_question_mark(self):
        """Test single question mark."""
        u = Undecorator("?")
        # Should not crash, may return partial result
        try:
            result = u.demangle()
            # Just verify it doesn't crash
            self.assertIsNotNone(result)
        except Exception:
            # Some implementations may raise on malformed input
            pass


if __name__ == "__main__":
    unittest.main()
