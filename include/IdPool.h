namespace MPL::Common
{
    struct IdPool
    {
        std::map<uint32_t, uint32_t> seg = {
            { 1, 0xFFFFFF - 1 }
        };
        uint32_t allocate()
        {
            auto it = seg.begin();
            uint32_t start = it->first;
            uint32_t len = it->second;
            if (len > 1)
            {
                seg[start + 1] = len - 1;
            }
            seg.erase(it);
            return start;
        }
        void release(uint32_t id)
        {
            auto next_it = seg.upper_bound(id);
            auto prev_it = (next_it != seg.begin()) ? std::prev(next_it) : seg.end();
            bool merge_prev = (prev_it != seg.end() && prev_it->first + prev_it->second == id);
            bool merge_next = (next_it != seg.end() && id + 1 == next_it->first);
            if (merge_prev && merge_next)
            {
                prev_it->second += 1 + next_it->second;
                seg.erase(next_it);
            }
            else if (merge_prev)
            {
                prev_it->second += 1;
            }
            else if (merge_next)
            {
                uint32_t next_len = next_it->second;
                seg.erase(next_it);
                seg[id] = next_len + 1;
            }
            else
            {
                seg[id] = 1;
            }
        }
    };
}  // namespace MPL::Common
