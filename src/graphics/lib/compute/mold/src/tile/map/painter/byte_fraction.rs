// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

//! Mold faction logic.
//!
//! Mold stores 0.0-1.0 fractions in `u8` bytes. Usual floating-point logic is translated into
//! usual unsigned integer logic.

use std::{
    fmt,
    ops::{Add, AddAssign, Mul, MulAssign, Sub, SubAssign},
};

use super::FillRule;

const LAST_BIT_MASK: i32 = 0b1;
const LAST_BYTE_MASK: i32 = 0b1111_1111;

#[repr(C)]
#[derive(Clone, Copy, Default, Eq, Ord, PartialEq, PartialOrd)]
pub struct ByteFraction {
    value: u8,
}

impl ByteFraction {
    pub const fn new(value: u8) -> Self {
        Self { value }
    }

    pub const fn zero() -> Self {
        Self { value: u8::min_value() }
    }

    pub const fn one() -> Self {
        Self { value: u8::max_value() }
    }

    pub fn from_area(mut area: i32, fill_rule: FillRule) -> Self {
        let value = match fill_rule {
            FillRule::NonZero => {
                if area < 0 {
                    area = -area;
                }

                if area >= 256 {
                    u8::max_value()
                } else {
                    area as u8
                }
            }
            FillRule::EvenOdd => {
                let number = area >> 8;

                if area < 0 && area & LAST_BYTE_MASK != 0 {
                    area -= 1;
                }

                let capped = area & LAST_BYTE_MASK;

                if number & LAST_BIT_MASK == 0 {
                    capped as u8
                } else {
                    u8::max_value() - capped as u8
                }
            }
        };

        Self { value }
    }

    pub fn value(self) -> u8 {
        self.value
    }
}

impl Add for ByteFraction {
    type Output = Self;

    fn add(self, rhs: Self) -> Self::Output {
        let value = self.value.saturating_add(rhs.value);
        Self { value }
    }
}

impl AddAssign for ByteFraction {
    fn add_assign(&mut self, rhs: Self) {
        *self = *self + rhs;
    }
}

impl Sub for ByteFraction {
    type Output = Self;

    fn sub(self, rhs: Self) -> Self::Output {
        let value = self.value.saturating_sub(rhs.value);
        Self { value }
    }
}

impl SubAssign for ByteFraction {
    fn sub_assign(&mut self, rhs: Self) {
        *self = *self - rhs;
    }
}

impl Mul for ByteFraction {
    type Output = Self;

    fn mul(self, rhs: Self) -> Self::Output {
        let product = u16::from(self.value) * u16::from(rhs.value);
        // Bitwise approximation of division by 255.
        let value = ((product + 128 + (product >> 8)) >> 8) as u8;
        Self { value }
    }
}

impl MulAssign for ByteFraction {
    fn mul_assign(&mut self, rhs: Self) {
        *self = *self * rhs;
    }
}

impl fmt::Debug for ByteFraction {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{:.3} ({}/255)", self.value as f32 / 255.0, self.value)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    const HALF: ByteFraction = ByteFraction::new(128);

    #[test]
    fn add() {
        assert_eq!(HALF + HALF, ByteFraction::one());
    }

    #[test]
    fn sub() {
        assert_eq!(HALF - HALF, ByteFraction::zero());
    }

    #[test]
    fn mul() {
        assert_eq!(HALF * HALF, ByteFraction::new(64));
    }

    #[test]
    fn non_zero() {
        assert_eq!(ByteFraction::from_area(257, FillRule::NonZero), ByteFraction::new(255));
        assert_eq!(ByteFraction::from_area(256, FillRule::NonZero), ByteFraction::new(255));
        assert_eq!(ByteFraction::from_area(255, FillRule::NonZero), ByteFraction::new(255));
        assert_eq!(ByteFraction::from_area(254, FillRule::NonZero), ByteFraction::new(254));
    }

    #[test]
    fn non_zero_negative() {
        assert_eq!(ByteFraction::from_area(-257, FillRule::NonZero), ByteFraction::new(255));
        assert_eq!(ByteFraction::from_area(-256, FillRule::NonZero), ByteFraction::new(255));
        assert_eq!(ByteFraction::from_area(-255, FillRule::NonZero), ByteFraction::new(255));
        assert_eq!(ByteFraction::from_area(-254, FillRule::NonZero), ByteFraction::new(254));
    }

    #[test]
    fn even_odd() {
        assert_eq!(ByteFraction::from_area(257, FillRule::EvenOdd), ByteFraction::new(254));
        assert_eq!(ByteFraction::from_area(256, FillRule::EvenOdd), ByteFraction::new(255));
        assert_eq!(ByteFraction::from_area(255, FillRule::EvenOdd), ByteFraction::new(255));
        assert_eq!(ByteFraction::from_area(254, FillRule::EvenOdd), ByteFraction::new(254));
    }

    #[test]
    fn even_odd_negative() {
        assert_eq!(ByteFraction::from_area(-257, FillRule::EvenOdd), ByteFraction::new(254));
        assert_eq!(ByteFraction::from_area(-256, FillRule::EvenOdd), ByteFraction::new(255));
        assert_eq!(ByteFraction::from_area(-255, FillRule::EvenOdd), ByteFraction::new(255));
        assert_eq!(ByteFraction::from_area(-254, FillRule::EvenOdd), ByteFraction::new(254));
    }
}
