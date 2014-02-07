#ifndef variablesFromCoordinates_h
#define variablesFromCoordinates_h

float pTRelFromCoordinates(const float& pT1, const float& eta1, const float& phi1,
                           const float& pT2, const float& eta2, const float& phi2);

float deltaRFromCoordinates(const float& pT1, const float& eta1, const float& phi1,
                            const float& pT2, const float& eta2, const float& phi2);

float deltaPhiFromCoordinates(const float& phi1, const float& phi2);

#endif
