#pragma once

char *serialize_node_data_to_mmap(rb_tree *tree);
void deserialize_node_data_from_mmap(rb_tree *tree, char *mmap_node_data);
