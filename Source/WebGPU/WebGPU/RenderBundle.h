/*
 * Copyright (c) 2021-2022 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#import "BindableResource.h"
#import <wtf/FastMalloc.h>
#import <wtf/Ref.h>
#import <wtf/RefCounted.h>
#import <wtf/Vector.h>

struct WGPURenderBundleImpl {
};

@interface ResourceUsageAndRenderStage : NSObject
- (instancetype)initWithUsage:(MTLResourceUsage)usage renderStages:(MTLRenderStages)renderStages;

@property (nonatomic) MTLResourceUsage usage;
@property (nonatomic) MTLRenderStages renderStages;
@end

@class RenderBundleICBWithResources;

namespace WebGPU {

class Device;

class RenderBundleEncoder;

// https://gpuweb.github.io/gpuweb/#gpurenderbundle
class RenderBundle : public WGPURenderBundleImpl, public RefCounted<RenderBundle> {
    WTF_MAKE_FAST_ALLOCATED;
public:
    using ResourcesContainer = NSMapTable<id<MTLResource>, ResourceUsageAndRenderStage*>;
    static Ref<RenderBundle> create(NSArray<RenderBundleICBWithResources*> *resources, RefPtr<WebGPU::RenderBundleEncoder> encoder, Device& device)
    {
        return adoptRef(*new RenderBundle(resources, encoder, device));
    }
    static Ref<RenderBundle> createInvalid(Device& device)
    {
        return adoptRef(*new RenderBundle(device));
    }

    ~RenderBundle();

    void setLabel(String&&);

    bool isValid() const { return m_renderBundlesResources != nil; }

    Device& device() const { return m_device; }
    NSArray<RenderBundleICBWithResources*> *renderBundlesResources() const { return m_renderBundlesResources; }

    bool replayCommands(id<MTLRenderCommandEncoder>) const;
private:
    RenderBundle(NSArray<RenderBundleICBWithResources*> *, RefPtr<RenderBundleEncoder>, Device&);
    RenderBundle(Device&);

    const Ref<Device> m_device;
    RefPtr<RenderBundleEncoder> m_renderBundleEncoder;
    NSArray<RenderBundleICBWithResources*> *m_renderBundlesResources;
};

} // namespace WebGPU
