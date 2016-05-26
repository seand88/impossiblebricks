//
//  AdMob.mm
//  VerticalVelocity
//
//  Created by Mike on 1/2/15.
//
//

#include "AdMob.h"
#include "AdMobWrapper.h"

namespace Social
{
    void AdMob::initBannerAd()
    {
        [[AdMobWrapper sharedInstance] initBannerAd];
    }
    
    void AdMob::showAd(bool show)
    {
        [[AdMobWrapper sharedInstance] showAd:show];
    }
}
