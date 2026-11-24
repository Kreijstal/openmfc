#pragma once

#include "openmfc/afxtoolbarfontcombobox.h"

#include <vector>

namespace openmfc::detail {

std::vector<CMFCFontInfo> CollectFontCatalog(int fontTypes, BYTE charSet, BYTE pitchAndFamily);

}
