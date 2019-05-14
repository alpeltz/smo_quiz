<template>
  <div id="score-card" class="score-card">
    <div class="total">
      <dl v-if="score.out_of >= 10">
        <dt>
          Score
        </dt>
        <dd>
          <span :class="grade( score ).class" v-tooltip="{ placement: 'left', content: grade( score ).tooltip, delay: { hide: 0, show: 0 } }">{{ grade( score ).letter }}</span>
        </dd>
      </dl>
      <dl v-if="score.out_of >= 1">
        <dt>
          Answers
        </dt>
        <dd>
          {{ score.out_of }}
        </dd>
      </dl>
      <dl @click="resetScore" v-if="score.out_of >= 1" class="toggle">
        <dt>Reset</dt>
      </dl>
      <dl @click="toggleMore" v-if="score.out_of >= 50" class="toggle">
        <dt>Show<br /> <template v-if="!showMore">More</template><template v-else>Less</template></dt>
      </dl>
    </div>

    <div class="by-type" v-if="showMore && score.out_of >= 50">
      <dl v-for="(type_score, name) in score.by_type" :key="name">
        <dt>
          {{ title[name] }}
        </dt>
        <dd>
          <span :class="grade( type_score, 5, 25 ).class" v-tooltip="{ placement: 'left', content: grade( type_score, 5, 25 ).tooltip, delay: { hide: 0, show: 0 } }">{{ grade( type_score, 5, 25 ).letter }}</span>
        </dd>
      </dl>
    </div>
  </div>
</template>
<script>
// import _ from 'lodash'

export default {
  name: 'ScoreCard',
  props: ['score'],
  data () {
    return {
      title: {
        guessTheMonk: 'Guess Monk by Trial',
        guessTheTrial: 'Guess Trial by Monk',
        guessTheShrine: 'Guess Shrine by Region',
        guessTheShrineFromQuest: 'Guess Shrine by Quest',
        guessTheShrineFromLandmark: 'Guess Shrine by Landmark',
        guessTheShrineFromLandmarkHard: 'Guess Shrine by Landmark [HARD]',
        guessTheLandmark: 'Guess Landmark',
        guessTheLandmarkHard: 'Guess Landmark [HARD]',
        guessTheQuest: 'Guess Quest',
        guessTheItem: 'Guess Item'
      },
      showMore: false
    }
  },
  methods: {
    toggleMore () {
      this.showMore = !this.showMore
    },
    grade (score, u = 10, star = 120) {
      let grade
      const percent = (score.score / score.out_of) * 100

      const gradeDef = {
        'U': 'Not enough data',
        'S': 'God-tier',
        'A': 'Great',
        'B': 'Good',
        'C': 'Fine',
        'D': 'Needs work',
        'E': 'Honestly better than nothing',
        'F': 'Keep at it',
        'Z': 'Z is the worst letter there is!'
      }

      if (score.out_of < u) {
        grade = 'U'
      } else if (percent >= 95) {
        grade = 'S'
      } else if (percent >= 86) {
        grade = 'A'
      } else if (percent >= 73) {
        grade = 'B'
      } else if (percent >= 60) {
        grade = 'C'
      } else if (percent >= 41) {
        grade = 'D'
      } else if (percent >= 26) {
        grade = 'E'
      } else if (percent >= 1) {
        grade = 'F'
      } else {
        grade = 'Z'
      }

      const starred = score.out_of >= star

      return {
        letter: grade,
        class: `grade${(starred) ? ' grade-star' : ''} grade-${grade.toLowerCase()}`,
        tooltip: `${gradeDef[grade]}${(starred) ? ` (Star for answering ${star}+)` : ''}`
      }
    },
    resetScore () {
      this.showMore = false
      this.$emit('reset')
    }
  }
}
</script>
<style>
  .score-card {
    position: fixed;
    top: 10px;
    right: 10px;
    display: flex;
    flex-direction: column;
    font-weight: bold;
    font-size: 16px;
    align-items: flex-end;
  }

  @media (max-width: 600px) {
    .score-card {
      font-size: 12px;
    }
  }

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
    .score-card .total dl {
      width: 60px;
      height: 60px;
    }
  }

  .score-card .total dl.toggle {
    cursor: pointer;
  }

  .score-card .total dl.toggle:hover {
    background: #fff;
    color: #060903;
  }

  .score-card .total dl + dl {
    border-left: 0;
  }

  .score-card .total dt {
    padding: 0 5px;
    margin: 0;
    font-size: 62.5%;
    text-transform: uppercase;
  }

  .score-card dd {
    margin: 0;
    font-size: 175%;
  }

  .score-card .by-type {
    display: flex;
    flex-direction: column;
    margin-top: 10px;
    background: #060903;
  }

  .score-card .by-type dl {
    display: flex;
    margin: 0;
    height: 60px;
    border: solid 1px #fff;
    justify-content: space-between;
    align-items: center;
  }

  @media (max-width: 600px) {
    .score-card .by-type dl {
      height: 40px;
    }
  }

  .score-card .by-type dl + dl {
    border-top: 0;
  }

  .score-card .by-type dt {
    padding: 5px 10px;
  }

  .score-card .by-type dd {
    width: 60px;
    height: 60px;
    border-left: solid 1px #fff;
    line-height: 60px;
  }

  @media (max-width: 600px) {
    .score-card .by-type dd {
      width: 40px;
      height: 40px;
      line-height: 40px;
    }
  }

  .score-card .grade {
    display: inline-block;
    width: 100%;
    cursor: help;
  }

  .score-card .by-type .grade {
    width: 60px;
    height: 60px;
  }

  @media (max-width: 600px) {
    .score-card .by-type .grade {
      width: 40px;
      height: 40px;
    }
  }

@keyframes rainbow {
    0% {
      color: #98d098
    }

    20% {
      color: #cfd098
    }

    40% {
      color: #d0aa98
    }

    60% {
      color: #d098cc
    }

    80% {
      color: #98c6d0
    }

    100% {
      color: #98d098
    }
  }

  .grade-star::after {
    display: inline-block;
    font-size: 40%;
    content: '★';
    vertical-align: middle;
  }

  .grade-s {
    color: #cfd098;
    animation: rainbow 5s ease infinite;
  }

  .grade-a {
    color: #98d098;
  }

  .grade-b {
    color: #cfd098;
  }

  .grade-c {
    color: #d0aa98;
  }

  .grade-d {
    color: #e89a9a;
  }

  .grade-e {
    color: #ed7491;
  }

  .grade-f {
    color: #9d485c;
  }

  .grade-u,
  .grade-z {
    color: #999;
  }

  .v-popper--theme-tooltip {
    display: none;
  }

  .v-popper__popper--open {
    display: block;
  }

  .v-popper--theme-tooltip .v-popper__inner {
    background: #060903;
    color: white;
    padding: 4px;
    max-width: 250px;
    border: solid 1px #fff;
  }

  .v-popper--theme-tooltip .v-popper__arrow {
    border-color: #fff;
  }
</style>
