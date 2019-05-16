<template>
  <div id="menu">
    <div class="options">
      <!--<div class="dropdown">
        <dl>
          <dt>Max Choices</dt>
          <dd>{{ options.chooseFrom }}</dd>
        </dl>

        <div class="dropdown-content">
          <div class="option" v-for="num in 7" :key="num" :class="{active: num + 1 === options.chooseFrom}" @click="changeOption('chooseFrom', num + 1)">
            {{ num + 1 }}
          </div>
        </div>
      </div>-->

      <div class="dropdown">
        <dl>
          <dt>Difficulty</dt>
          <dd class="small">{{ options.difficulty | capitalize }}</dd>
        </dl>

        <div class="dropdown-content">
          <div class="option" v-for="difficulty in difficulties" :key="difficulty" :class="{active: difficulty === options.difficulty}" @click="changeOption('difficulty', difficulty)">
            {{ difficulty | capitalize }}
          </div>
        </div>
      </div>

      <!--<dl class="toggle" @click="toggleOption('includeDLC')">
        <dt>DLC</dt>
        <dd>{{ options.includeDLC ? 'ON' : 'OFF' }}</dd>
      </dl>-->

      <dl class="toggle" @click="toggleOption('soundOn', false)">
        <dt>Sounds</dt>
        <dd>{{ options.soundOn ? 'ON' : 'OFF' }}</dd>
      </dl>
    </div>

    <score-card v-if="score.out_of > 0" :score="score" @reset="$emit('reset')"></score-card>
  </div>
</template>
<script>
import ScoreCard from './ScoreCard.vue'

export default {
  name: 'AppMenu',
  components: {
    ScoreCard
  },
  props: {
    options: {
      type: Object,
      default: () => {
        return {
          soundOn: true,
          includeDLC: false,
          chooseFrom: 4,
          difficulty: 'normal'
        }
      }
    },
    score: {
      type: Object,
      default: () => {
        return {
          score: 0,
          out_of: 0,
          by_type: {}
        }
      }
    }
  },
  data () {
    return {
      difficulties: ['easy', 'normal', 'hard']
    }
  },
  methods: {
    toggleOption (option, newQ = true) {
      this.options[option] = !this.options[option]
      if (newQ) {
        this.$refs.quiz.newQuestion()
      }
    },
    changeOption (option, value) {
      if (this.options[option] !== value && this.resetScore()) {
        this.options[option] = value
        this.$refs.quiz.newQuestion()
      }
    }
  },
  filters: {
    capitalize (value) {
      return value.charAt(0).toUpperCase() + value.slice(1)
    }
  }
}
</script>

<style>

#menu {
  display: flex;
  position: fixed;
  top: 10px;
  right: 10px;
  left: 10px;
  z-index: 99;
  font-weight: bold;
  font-size: 16px;
  justify-content: space-between;
}

@media (max-width: 600px) {
  #menu {
    top: 0;
    right: 0;
    left: 0;
    background: #060903;
    border-bottom: solid 1px #fff;
  }
}

.options {
  display: flex;
}

@media (max-width: 600px) {
  .options {
    font-size: 12px;
  }
}

.options dl,
.score-card .total dl {
  display: flex;
  flex-direction: column;
  float: left;
  margin: 0;
  padding: 5px 0;
  width: 80px;
  height: 80px;
  border: solid 1px #fff;
  background: #060903;
  text-align: center;
  justify-content: center;
}

@media (max-width: 600px) {
  .options dl,
  .score-card .total dl {
    border-top: 0;
    border-left: 0;
    border-bottom: 0;
  }

  .score-card .total dl {
    border-left: solid 1px #fff;
    border-right: 0;
  }
}

@media (max-width: 600px) {
  .options dl {
    width: 60px;
    height: 60px;
  }
}

.options dl + dl,
.options .dropdown + dl,
.options dl + .dropdown > dl,
.options .dropdown + .dropdown > dl {
  border-left: 0;
}

.options dt {
  padding: 0 5px;
  margin: 0;
  font-size: 62.5%;
  text-transform: uppercase;
}

.options dd {
  margin: 0;
  font-size: 175%;
}

.options dd.small {
  margin: 0;
  font-size: 100%;
}

.options dl.toggle,
.options .dropdown dl {
  cursor: pointer;
}

.options dl.toggle:hover,
.options .dropdown dl:hover {
  background: #fff;
  color: #060903;
}

.options .dropdown {
  position: relative;
}

.options .dropdown:hover .dropdown-content {
  display: block;
}

.options .dropdown .dropdown-content {
  display: none;
  position: absolute;
  top: 100%;
  left: 0;
  padding: 2px 8px;
  min-width: 100%;
  border: solid 1px #fff;
  background: #060903;
  text-align: left;
  font-size: 16px;
}

@media (max-width: 600px) {
  .options .dropdown .dropdown-content .option {
    padding: .5em 0;
  }
}

.options .dropdown .option {
  cursor: pointer;
  color: rgba(255,255,255, 0.5);
}

.options .dropdown .option {
  cursor: pointer;
}

.options .dropdown .option:hover,
.options .dropdown .option.active {
  color: #fff;
}
</style>
