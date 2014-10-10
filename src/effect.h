#ifndef EFFECT_H
#define EFFECT_H

#include "pldata.h"
#include "json.h"
#include "messages.h"

class effect_type;
class Creature;

extern std::map<std::string, effect_type> effect_types;

enum effect_rating {
    e_good, // the effect is good for the one who has it.
    e_neutral,  // there is no effect or the effect is very nominal. This is the default.
    e_bad,      // the effect is bad for the one who has it
    e_mixed     // the effect has good and bad parts to the one who has it
};

struct effect_mod_info {
    /** All pairs are unmodified,reduced */
    std::pair<float, float> str_mod;
    std::pair<float, float> dex_mod;
    std::pair<float, float> per_mod;
    std::pair<float, float> int_mod;
    std::pair<int, int> speed_mod;
    
    std::pair<int, int> pain_amount;
    std::pair<int, int> pain_min;
    std::pair<int, int> pain_max;
    std::pair<int, int> pain_max_val;
    std::pair<int, int> pain_chance_top;
    std::pair<int, int> pain_chance_bot;
    std::pair<int, int> pain_tick;

    std::pair<int, int> hurt_amount;
    std::pair<int, int> hurt_min;
    std::pair<int, int> hurt_max;
    std::pair<int, int> hurt_chance_top;
    std::pair<int, int> hurt_chance_bot;
    std::pair<int, int> hurt_tick;
    
    std::pair<int, int> pkill_amount;
    std::pair<int, int> pkill_min;
    std::pair<int, int> pkill_max;
    std::pair<int, int> pkill_max_val;
    std::pair<int, int> pkill_chance_top;
    std::pair<int, int> pkill_chance_bot;
    std::pair<int, int> pkill_tick;

    std::pair<int, int> cough_chance_top;
    std::pair<int, int> cough_chance_bot;
    std::pair<int, int> cough_tick;
    
    std::pair<int, int> vomit_chance_top;
    std::pair<int, int> vomit_chance_bot;
    std::pair<int, int> vomit_tick;
    
    bool load(JsonObject &jsobj, std::string member);
};

class effect_type
{
        friend void load_effect_type(JsonObject &jo);
        friend class effect;
    public:
        effect_type();
        effect_type(const effect_type &rhs);

        efftype_id id;

        effect_rating get_rating() const;
        
        bool use_name_ints();
        bool use_desc_ints();

        // Appropriate game_message_type when effect is optained.
        game_message_type gain_game_message_type() const;
        // Appropriate game_message_type when effect is lost.
        game_message_type lose_game_message_type() const;

        std::string get_apply_message() const;
        std::string get_apply_memorial_log() const;
        std::string get_remove_message() const;
        std::string get_remove_memorial_log() const;

        int get_max_intensity() const;

    protected:
        bool permanent;
        int max_intensity;
        int dur_add_perc;
        int int_add_val;
        int int_decay_step;
        int int_decay_tick;
        bool main_parts_only;
        std::string resist_trait;
        
        bool pain_sizing;
        bool hurt_sizing;
        bool harmful_cough;
        // Once addictions are JSON-ized it should be trivial to convert this to a 
        // "generic" addiction reduces value
        bool pkill_addict_reduces;

        std::vector<std::string> name;
        std::string speed_mod_name;
        std::vector<std::string> desc;
        std::vector<std::string> reduced_desc;
        bool part_descs;

        effect_rating rating;

        std::string apply_message;
        std::string apply_memorial_log;
        std::string remove_message;
        std::string remove_memorial_log;
        
        effect_mod_info base_mods;
        effect_mod_info scaling_mods;
};

class effect : public JsonSerializer, public JsonDeserializer
{
    public:
        effect();
        effect(effect_type *eff_type, int dur, body_part part, int nintensity, bool perm);
        effect(const effect &rhs);
        effect &operator=(const effect &rhs);

        std::string disp_name();
        std::string disp_desc(bool reduced = false);
        bool use_part_descs();

        effect_type *get_effect_type();

        int get_duration();
        void set_duration(int dur);
        void mod_duration(int dur);
        
        body_part get_bp();
        void set_bp(body_part part);

        bool is_permanent();
        void pause_effect();
        void unpause_effect();

        int get_intensity();
        int get_max_intensity();
        void set_intensity(int nintensity);
        void mod_intensity(int nintensity);
        
        std::string get_resist_trait();
        
        int get_mod(std::string arg, bool reduced = false);
        int get_amount(std::string arg, bool reduced = false);
        int get_max_val(std::string arg, bool reduced = false);
        bool get_sizing(std::string arg);
        /** mod modifies x in x_in_y or one_in(x) */
        bool activated(unsigned int turn, std::string arg, bool reduced = false, double mod = 1);
        
        double get_addict_reduction(std::string arg, int addict_level);
        bool get_harmful_cough();

        efftype_id get_id()
        {
            return eff_type->id;
        }

        using JsonSerializer::serialize;
        void serialize(JsonOut &json) const
        {
            json.start_object();
            json.member("eff_type", eff_type != NULL ? eff_type->id : "");
            json.member("duration", duration);
            json.member("bp", (int)bp);
            json.member("intensity", intensity);
            json.member("permanent", permanent);
            json.end_object();
        }
        using JsonDeserializer::deserialize;
        void deserialize(JsonIn &jsin)
        {
            JsonObject jo = jsin.get_object();
            eff_type = &effect_types[jo.get_string("eff_type")];
            duration = jo.get_int("duration");
            bp = (body_part)jo.get_int("bp");
            intensity = jo.get_int("intensity");
            permanent = jo.get_bool("permanent");
        }

    protected:
        effect_type *eff_type;
        int duration;
        body_part bp;
        int intensity;
        bool permanent;

};

void load_effect_type(JsonObject &jo);
void reset_effect_types();

#endif
