/*
 * Copyright (C) 2023 Apple Inc. All rights reserved.
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
#include "config.h"

#if ENABLE(WEBGL)
#include "WebGLDefaultFramebuffer.h"

namespace WebCore {

std::unique_ptr<WebGLDefaultFramebuffer> WebGLDefaultFramebuffer::create(WebGLRenderingContextBase& context, IntSize size)
{
    auto instance = std::unique_ptr<WebGLDefaultFramebuffer> { new WebGLDefaultFramebuffer(context) };
    instance->reshape(size);
    return instance;
}

WebGLDefaultFramebuffer::WebGLDefaultFramebuffer(WebGLRenderingContextBase& context)
    : m_context(context)
{
    auto attributes = m_context.graphicsContextGL()->contextAttributes();
    m_hasStencil = attributes.stencil;
    m_hasDepth = attributes.depth;
    if (!attributes.preserveDrawingBuffer) {
        m_unpreservedBuffers = GraphicsContextGL::COLOR_BUFFER_BIT;
        if (m_hasStencil)
            m_unpreservedBuffers |= GraphicsContextGL::STENCIL_BUFFER_BIT;
        if (m_hasDepth)
            m_unpreservedBuffers |= GraphicsContextGL::DEPTH_BUFFER_BIT;
    }
}

void WebGLDefaultFramebuffer::reshape(IntSize size)
{
    m_context.graphicsContextGL()->reshape(size.width(), size.height());
}

void WebGLDefaultFramebuffer::markBuffersClear(GCGLbitfield clearBuffers)
{
    m_dirtyBuffers &= ~clearBuffers;
}

void WebGLDefaultFramebuffer::markAllUnpreservedBuffersDirty()
{
    m_dirtyBuffers = m_unpreservedBuffers;
}

}

#endif
