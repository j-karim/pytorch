#pragma once

#ifdef USE_VULKAN_API

#include <ATen/native/vulkan/ops/Common.h>
#include <ATen/native/vulkan/ops/VulkanPackedContext.h>
#include <torch/library.h>

namespace at {
namespace native {
namespace vulkan {
namespace ops {

class LstmPackedContext final : virtual public VulkanPackedContext,
                                public torch::jit::CustomClassHolder {
 public:
  LstmPackedContext(
      const std::vector<Tensor>& params_cpu, // weights/biases (cpu)
      bool has_biases,
      int64_t num_layers,
      double dropout,
      bool train,
      bool bidirectional,
      bool batch_first);

  /*
   * Assigns a name to each index in the unpacked list.
   */
  struct Unpacked final {
    static constexpr uint32_t Params = 0u;
    static constexpr uint32_t hasBiases = 1u;
    static constexpr uint32_t NumLayers = 2u;
    static constexpr uint32_t Dropout = 3u;
    static constexpr uint32_t Train = 4u;
    static constexpr uint32_t Bidirectional = 5u;
    static constexpr uint32_t BatchFirst = 6u;
  };

  /*
   * Assigns a name to each index in the packed list.
   */
  struct Packed final {
    static constexpr uint32_t LinearContexts = 0u;
    static constexpr uint32_t hasBiases = 1u;
    static constexpr uint32_t NumLayers = 2u;
    static constexpr uint32_t Dropout = 3u;
    static constexpr uint32_t Train = 4u;
    static constexpr uint32_t Bidirectional = 5u;
    static constexpr uint32_t BatchFirst = 6u;
  };

  static LstmPackedContext pack(c10::impl::GenericList);

  const c10::impl::GenericList unpack() const override;
};

c10::intrusive_ptr<LstmPackedContext> create_lstm_context(
    std::vector<Tensor>&& params_cpu, // weights/biases (cpu)
    bool has_biases,
    int64_t num_layers,
    double dropout,
    bool train,
    bool bidirectional,
    bool batch_first);

std::tuple<Tensor, Tensor, Tensor> run_lstm_context(
    const Tensor& input_vk, // input sequence (vulkan)
    const Tensor& hx_vk, // initial hidden state (vulkan)
    const Tensor& cx_vk, // initial cell state (vulkan)
    const c10::intrusive_ptr<LstmPackedContext>& vulkan_context);

} // namespace ops
} // namespace vulkan
} // namespace native
} // namespace at

#endif /* USE_VULKAN_API */
