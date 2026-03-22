#include "RenderCommand.h"

namespace Nullcore {

	std::unique_ptr<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}