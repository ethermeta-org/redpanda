/*
 * Copyright 2020 Vectorized, Inc.
 *
 * Use of this software is governed by the Business Source License
 * included in the file licenses/BSL.md
 *
 * As of the Change Date specified in that file, in accordance with
 * the Business Source License, use of this software will be governed
 * by the Apache License, Version 2.0
 */

#include "cluster/scheduling/types.h"

#include "cluster/scheduling/partition_allocator.h"

#include <fmt/ostream.h>

namespace cluster {
std::ostream& operator<<(std::ostream& o, const allocation_constraints& a) {
    fmt::print(
      o,
      "{{soft_constraints: {}, hard_constraints: {}}}",
      a.soft_constraints,
      a.hard_constraints);
    return o;
}

void allocation_constraints::add(allocation_constraints other) {
    std::move(
      other.hard_constraints.begin(),
      other.hard_constraints.end(),
      std::back_inserter(hard_constraints));

    std::move(
      other.soft_constraints.begin(),
      other.soft_constraints.end(),
      std::back_inserter(soft_constraints));
}

allocation_units::~allocation_units() {
    for (auto& pas : _assignments) {
        for (auto& replica : pas.replicas) {
            _allocator->deallocate(replica);
        }
    }
}

partition_constraints::partition_constraints(
  model::partition_id id, uint16_t replication_factor)
  : partition_constraints(id, replication_factor, allocation_constraints{}) {}

partition_constraints::partition_constraints(
  model::partition_id id,
  uint16_t replication_factor,
  allocation_constraints constraints)
  : partition_id(id)
  , replication_factor(replication_factor)
  , constraints(std::move(constraints)) {}

std::ostream& operator<<(std::ostream& o, const partition_constraints& pc) {
    fmt::print(
      o,
      "{{partition_id: {}, replication_factor: {}, constrains: {}}}",
      pc.partition_id,
      pc.replication_factor,
      pc.constraints);
    return o;
}
std::ostream& operator<<(std::ostream& o, const allocation_request& req) {
    fmt::print(o, "{{partion_constraints: {}}}", req.partitions);
    return o;
}
} // namespace cluster
