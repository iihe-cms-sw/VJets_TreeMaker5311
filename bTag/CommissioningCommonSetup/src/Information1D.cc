#include "bTag/CommissioningCommonSetup/interface/Information1D.h"

#include <algorithm>

Information1D::Information1D() : BaseInformation(), yTitle(""), jetCut(""), trackCut(""), yMin(0.0), ratiomin(0.0),
                                 ratiomax(5.0), bscale(1.0), cscale(1.0), ratiorebin(0), displayOverUnderflowBin(false),
                                 displayNoInfo(false), legendPosition(false) {}
