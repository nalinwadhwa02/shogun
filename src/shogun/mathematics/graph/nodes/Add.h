/*
 * This software is distributed under BSD 3-clause license (see LICENSE file).
 *
 * Authors: Gil Hoben
 */

#ifndef SHOGUNTENSORADD_H_
#define SHOGUNTENSORADD_H_

#include <shogun/mathematics/graph/nodes/BinaryNode.h>

#define IGNORE_IN_CLASSLIST

namespace shogun
{
	IGNORE_IN_CLASSLIST class Add : public BinaryNode
	{
	public:
		Add(const std::shared_ptr<Node>& node1,
		    const std::shared_ptr<Node>& node2): BinaryNode(node1, node2)
		{
		}

		std::string to_string() const override
		{
			return fmt::format("Add(shape={}, type={})", get_tensors()[0]->get_shape(), get_tensors()[0]->get_type());
		}

		std::string_view get_operator_name() const override
		{
			return "Add";
		}
	};
} // namespace shogun

#endif