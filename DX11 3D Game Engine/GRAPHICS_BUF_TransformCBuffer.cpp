#include "GRAPHICS_BUF_TransformCbuffer.h"
namespace GPipeline
{
	TransformCbuffer::TransformCbuffer(Graphics& gfx, UINT slot)
	{
		// if the shared temp space haven't been assigned
		if (!pVcbFinalMatrix)
		{
			pVcbFinalMatrix = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
		}
	}

	void TransformCbuffer::Bind(Graphics& gfx) noexcept
	{
		UpdateBindImpl(gfx, GetTransforms(gfx));
	}

	void TransformCbuffer::InitializeParentReference(const Drawable& drawTarget) noexcept
	{
		pDrawTarget = &drawTarget;
	}

	void TransformCbuffer::UpdateBindImpl(Graphics & gfx, const Transforms & tf) noexcept
	{
		assert(pDrawTarget != nullptr);
		pVcbFinalMatrix->Update(gfx, tf);
		pVcbFinalMatrix->Bind(gfx);
	}

	TransformCbuffer::Transforms TransformCbuffer::GetTransforms(Graphics & gfx) noexcept
	{
		assert(pDrawTarget != nullptr);
		const auto modelView = pDrawTarget->GetTransformXM() * gfx.GetCamera();
		return {
			DirectX::XMMatrixTranspose(modelView),
			DirectX::XMMatrixTranspose(
				modelView *
				gfx.GetProjection()
			)
		};
	}

	std::unique_ptr<VertexConstantBuffer<TransformCbuffer::Transforms>> TransformCbuffer::pVcbFinalMatrix;
}