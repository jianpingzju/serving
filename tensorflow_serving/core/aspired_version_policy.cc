/* Copyright 2017 Google Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <unordered_set>

#include "tensorflow_serving/core/aspired_version_policy.h"

namespace tensorflow {
namespace serving {

absl::optional<ServableId> AspiredVersionPolicy::GetHighestAspiredNewServableId(
    const std::vector<AspiredServableStateSnapshot>& all_versions) {
  absl::optional<ServableId> highest_version_id;
  for (const auto& version : all_versions) {
    if (version.is_aspired && version.state == LoaderHarness::State::kNew) {
      if (!highest_version_id ||
          version.id.version > highest_version_id.value().version) {
        highest_version_id = version.id;
      }
    }
  }
  return highest_version_id;
}

std::unordered_set<int64>
AspiredVersionPolicy::GetSpecificVersionsInConfig(
    const std::string& servable_name) const {
  mutex_lock l(mu_);
  if (storage_path_source_ == nullptr) {
    std::unordered_set<int64> empty_set;
    return empty_set;
  }
  return storage_path_source_->GetSpecificVersionsInConfig(servable_name);
}

}  // namespace serving
}  // namespace tensorflow
