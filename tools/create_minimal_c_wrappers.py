#!/usr/bin/env python3
"""
Generate minimal C wrappers for critical MFC functions.
"""

import json
import sys
from pathlib import Path

def generate_afx_throw_wrappers(mapping_file, output_cpp, output_h):
    """
    Generate C wrappers for AfxThrow* functions.
    """
    with open(mapping_file, 'r') as f:
        data = json.load(f)
    
    exports = data.get('exports', {}).get('mfc140u', [])
    
    # Find AfxThrow functions
    afx_throw_funcs = []
    for export in exports:
        symbol = export['symbol']
        if 'AfxThrow' in symbol and '@@YA' in symbol:
            afx_throw_funcs.append({
                'symbol': symbol,
                'ordinal': export['ordinal']
            })
    
    print(f"Found {len(afx_throw_funcs)} AfxThrow functions")
    
    # Generate header
    header_content = """// Minimal C Wrappers for MFC AfxThrow Functions
// Provides C linkage for MSVC/MinGW interoperability

#ifndef OPENMFC_C_WRAPPERS_MINIMAL_H
#define OPENMFC_C_WRAPPERS_MINIMAL_H

#ifdef __cplusplus
extern "C" {
#endif

"""
    
    # Generate implementation
    impl_content = """// Minimal C Wrappers for MFC AfxThrow Functions
// Generated from MSVC ordinal mapping

#include "mfc_c_wrappers_minimal.h"

"""
    
    for func in afx_throw_funcs:
        symbol = func['symbol']
        ordinal = func['ordinal']
        
        # Extract function name (between ? and @@)
        start = symbol.find('?') + 1
        end = symbol.find('@@')
        if start > 0 and end > start:
            func_name = symbol[start:end]
        else:
            func_name = f"func_{ordinal}"
        
        # Create C wrapper name
        c_name = f"openmfc_{func_name.lower()}"
        
        # Check if it's void with no args
        if '@@YAXXZ' in symbol:
            # void __cdecl with no args
            header_content += f"void {c_name}(void);\n"
            
            # The stub function name (from stubs.cpp)
            stub_name = f"stub_{func_name}"
            
            impl_content += f"""
// Wrapper for {symbol} (ordinal {ordinal})
void {c_name}(void)
{{
    extern "C" void {stub_name}(void);
    {stub_name}();
}}
"""
        else:
            # Has arguments - create stub
            header_content += f"void {c_name}_stub(void);  // TODO: Implement {func_name}\n"
            
            impl_content += f"""
// Wrapper stub for {symbol} (ordinal {ordinal})
void {c_name}_stub(void)
{{
    // TODO: Implement wrapper for {func_name}
}}
"""
    
    header_content += """
#ifdef __cplusplus
}
#endif

#endif // OPENMFC_C_WRAPPERS_MINIMAL_H
"""
    
    # Write files
    with open(output_h, 'w') as f:
        f.write(header_content)
    
    with open(output_cpp, 'w') as f:
        f.write(impl_content)
    
    print(f"Generated {len(afx_throw_funcs)} C wrappers")
    print(f"Header: {output_h}")
    print(f"Implementation: {output_cpp}")

def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <mapping.json>")
        print(f"Example: {sys.argv[0]} mfc_complete_ordinal_mapping.json")
        sys.exit(1)
    
    mapping_file = sys.argv[1]
    
    if not Path(mapping_file).exists():
        print(f"Error: Mapping file not found: {mapping_file}")
        sys.exit(1)
    
    generate_afx_throw_wrappers(
        mapping_file,
        'src/mfc_c_wrappers_minimal.cpp',
        'src/mfc_c_wrappers_minimal.h'
    )

if __name__ == '__main__':
    main()