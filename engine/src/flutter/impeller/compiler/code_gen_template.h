// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <string_view>

namespace impeller {
namespace compiler {

constexpr std::string_view kReflectionHeaderTemplate =
    R"~~(// THIS FILE IS GENERATED BY impellerc.
// DO NOT EDIT OR CHECK THIS INTO SOURCE CONTROL

#pragma once

#include "shader_types.h"

namespace impeller {
namespace shader {

struct {{camel_case(shader_name)}}{{camel_case(shader_stage)}}Info {
  // ===========================================================================
  // Stage Info ================================================================
  // ===========================================================================
  static constexpr std::string_view kLabel = "{{camel_case(shader_name)}}";
  static constexpr std::string_view kEntrypointName = "{{entrypoint}}";
  static constexpr ShaderStage kShaderStage = {{to_shader_stage(shader_stage)}};
{% if length(struct_definitions) > 0 %}
  // ===========================================================================
  // Struct Definitions ========================================================
  // ===========================================================================
{% for def in struct_definitions %}

  struct {{def.name}} {
{% for member in def.members %}
    {{member.type}} {{member.name}};
{% endfor %}
  }; // struct {{def.name}}
{% endfor %}
{% endif %}
{% if length(uniform_buffers) > 0 %}
  // ===========================================================================
  // Stage Uniforms ============================================================
  // ===========================================================================
{% for uniform in uniform_buffers %}

  static constexpr auto kUniform{{camel_case(uniform.name)}} = ShaderUniformSlot<{{uniform.name}}> { // {{uniform.name}}
    "{{uniform.name}}",     // name
    {{uniform.binding}}u,   // binding
  };
{% endfor %}
{% endif %}
  // ===========================================================================
  // Stage Inputs ==============================================================
  // ===========================================================================
{% if length(stage_inputs) > 0 %}
{% for stage_input in stage_inputs %}

  static constexpr auto kInput{{camel_case(stage_input.name)}} = ShaderStageIOSlot { // {{stage_input.name}}
    "{{stage_input.name}}",             // name
    {{stage_input.location}}u,          // attribute location
    {{stage_input.descriptor_set}}u,    // attribute set
    {{stage_input.binding}}u,           // attribute binding
    {{stage_input.type.type_name}},     // type
    {{stage_input.type.bit_width}}u,    // bit width of type
    {{stage_input.type.vec_size}}u,     // vec size
    {{stage_input.type.columns}}u       // number of columns
  };
{% endfor %}
{% endif %}

  static constexpr std::array<const ShaderStageIOSlot*, {{length(stage_inputs)}}> kAllShaderStageInputs = {
{% for stage_input in stage_inputs %}
    &kInput{{camel_case(stage_input.name)}}, // {{stage_input.name}}
{% endfor %}
  };

{% if length(sampled_images) > 0 %}
  // ===========================================================================
  // Sampled Images ============================================================
  // ===========================================================================
{% for sampled_image in sampled_images %}

  static constexpr auto kInput{{camel_case(sampled_image.name)}} = ShaderStageIOSlot { // {{sampled_image.name}}
    "{{sampled_image.name}}",             // name
    {{sampled_image.location}}u,          // attribute location
    {{sampled_image.descriptor_set}}u,    // attribute set
    {{sampled_image.binding}}u,           // attribute binding
    {{sampled_image.type.type_name}},     // type
    {{sampled_image.type.bit_width}}u,    // bit width of type
    {{sampled_image.type.vec_size}}u,     // vec size
    {{sampled_image.type.columns}}u       // number of columns
  };
{% endfor %}
{% endif %}
  // ===========================================================================
  // Stage Outputs =============================================================
  // ===========================================================================
{% if length(stage_outputs) > 0 %}
{% for stage_output in stage_outputs %}
  static constexpr auto kOutput{{camel_case(stage_output.name)}} = ShaderStageIOSlot { // {{stage_output.name}}
    "{{stage_output.name}}",             // name
    {{stage_output.location}}u,          // attribute location
    {{stage_output.descriptor_set}}u,    // attribute set
    {{stage_output.binding}}u,           // attribute binding
    {{stage_output.type.type_name}},     // type
    {{stage_output.type.bit_width}}u,    // bit width of type
    {{stage_output.type.vec_size}}u,     // vec size
    {{stage_output.type.columns}}u       // number of columns
  };
{% endfor %}
  static constexpr std::array<const ShaderStageIOSlot*, {{length(stage_outputs)}}> kAllShaderStageOutputs = {
{% for stage_output in stage_outputs %}
    &kOutput{{camel_case(stage_output.name)}}, // {{stage_output.name}}
{% endfor %}
  };
{% endif %}

}; // struct {{camel_case(shader_name)}}{{camel_case(shader_stage)}}Info

} // namespace shader
} // namespace impeller
)~~";

constexpr std::string_view kReflectionCCTemplate =
    R"~~(// THIS FILE IS GENERATED BY impellerc.
// DO NOT EDIT OR CHECK THIS INTO SOURCE CONTROL

#include "{{header_file_name}}"

#include <type_traits>

namespace impeller {
namespace shader {

using Info = {{camel_case(shader_name)}}{{camel_case(shader_stage)}}Info;

{% for def in struct_definitions %}
// Sanity checks for {{def.name}}
static_assert(std::is_standard_layout_v<Info::{{def.name}}>);
static_assert(sizeof(Info::{{def.name}}) == {{def.byte_length}});
{% for member in def.members %}
static_assert(offsetof(Info::{{def.name}}, {{member.name}}) == {{member.offset}});
{% endfor %}
{% endfor %}


} // namespace shader
} // namespace impeller
)~~";

}  // namespace compiler
}  // namespace impeller
